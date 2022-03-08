#include <stdio.h>
#include <string>

#include "rapidcsv.h"
#include "tinyxml2.h"

using namespace tinyxml2;

int convert(const char* input_file_path, const char* output_file_path, const char* root_name, const char* element_name, char sep, bool use_attributes)
{
	// TODO: Handle getting input from stdin. We'll have to collect all of the input as a string then create a stringstream for rapidcsv

	// Open CSV document
	rapidcsv::Document csv_doc(input_file_path, rapidcsv::LabelParams(-1, -1), rapidcsv::SeparatorParams(sep));

	// Get headers, which will be used as sub-elements in each element
	std::vector<std::string> headers = csv_doc.GetRow<std::string>(0);

	// XML doesn't take kindly to spaces in elements/headers, so replace them with underscores
	for (int i = 0; i < headers.size(); i++)
	{
		std::replace(headers[i].begin(), headers[i].end(), ' ', '_');

		// TODO: investigate if there are other possible invalid characters e.g. quotes
		// We could aggressively remove anything other than alphanumeric characters plus dash and underscore, but then this will only work for english CSVs
		// Need to reference the XML spec

		// For now just remove quotes
		std::remove(headers[i].begin(), headers[i].end(), '\"');
		std::remove(headers[i].begin(), headers[i].end(), '\'');

		// TODO: throw warning if invalid characters were used? Ideally, CSVs should be well-formed from the start
	}

	// Open XML document
	XMLDocument xml_doc;
	XMLNode *xml_root = xml_doc.NewElement(root_name);
	xml_doc.InsertFirstChild(xml_root);

	// Find number of elements in CSV
	std::vector<std::string> temp_rows = csv_doc.GetColumn<std::string>(0);
	size_t num_elements = temp_rows.size(); // technically this is the number of elements plus one for the header row

	for (size_t i = 1; i < num_elements; i++)
	{
		// Create a new element
		XMLElement *xml_element = xml_doc.NewElement(element_name);

		// Add headers as sub-elements/attributes
		for (size_t j = 0; j < headers.size(); j++)
		{
			std::string text = csv_doc.GetCell<std::string>(j, i);

			// Skip making elements/attributes for empty cells
			if (text == "") continue;

			if (use_attributes)
			{
				xml_element->SetAttribute(headers[j].c_str(), text.c_str());
			}
			else
			{
				XMLElement *sub_element = xml_doc.NewElement(headers[j].c_str());
				sub_element->SetText(text.c_str());
				xml_element->InsertEndChild(sub_element);
			}
		}

		// Add element to root
		xml_root->InsertEndChild(xml_element);
	}

	if (strcmp(output_file_path, "stdout") == 0) // hack
	{
		XMLPrinter printer(stdout);
		xml_doc.Print(&printer);
	}

	else // For "real" files
	{
		XMLError xml_result = xml_doc.SaveFile(output_file_path);

		if (xml_result != XML_SUCCESS)
		{
			printf("ERROR: Unable to save XML document %s\n", output_file_path);
			return xml_result;
		}
	}

	// TODO: Figure out inserting XML version header. Not sure why tinyXML2 doesn't do this automatically
	// Might need to do a slightly ugly hack and open the file, insert it at the top, then close file

	return EXIT_SUCCESS;
}