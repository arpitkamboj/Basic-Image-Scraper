#include "splashkit.h"

using namespace std;

int main()
{
    write_line("Enter Search Query:");
    string query = read_line();
    write_line("");

    //Saving the original query with spaces for later use in logging.
    string final_query = query;

    //This while loop runs until there are no spaces left in the query
    //string::npos is a way to denote no-position. It basically means the specified string was not present in the main string.
    while(final_query.find(" ") != string::npos){
        //replace() takes 3 arguments - starting position, length and replacement
        //In this example, we tell replace() to replace all spaces with %20 (HTML encoded space)
        final_query.replace(final_query.find(" "), 1, "%20");
    }

    write_line("Searching IMGUR for: " + query);
    write_line("");

    http_response search_result;
    search_result = http_get("https://imgur.com/search/score?q=" + final_query, 443);
    string response = http_response_to_string(search_result);
    
    //Delimiter is a string that defines the position for splitting.
    string delimiter = "i.imgur.com/";
    string delimiter_end = "b.jpg";

    //Checking if there are no image results found
    if (response.find(delimiter) != string::npos) {
        write_line("Image found! Downloading...");

        //substr() takes 2 arguments - starting position and length of string to extract
        //In this example, substr is told to extract string from i.imgur all the way up to the unique image number
        string image_URL = response.substr(response.find(delimiter), response.find(delimiter_end) - response.find(delimiter));

        bitmap output_image = download_bitmap("result", "https://" + image_URL + ".jpg", 443);
        save_bitmap(output_image, query);
        write_line("Image saved to desktop.");
    } else {
        write_line("No results found.");
    }
    
    return 0;
}