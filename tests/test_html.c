#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../split_html.h" // Assuming split_HTML function is declared in split_html.h

// Helper function to create a temporary HTML file with given content
void create_temp_html_file(const char* filename, const char* content) {
    FILE* tmp_html = fopen(filename, "w");
    if (tmp_html == NULL) {
        perror("Failed to open file for writing");
        exit(EXIT_FAILURE);
    }
    fputs(content, tmp_html);
    fclose(tmp_html);
}

// Test case: Splitting HTML with a single large tag exceeding max_len
void test_split_HTML_large_single_tag() {
    const char* html_content = "<div>";
    const int tag_length = 10000; // Length of the large tag
    char* closing_tag = "</div>";
    char* large_html = malloc(strlen(html_content) + tag_length + strlen(closing_tag) + 1);

    if (large_html == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    strcpy(large_html, html_content);
    for (int i = 0; i < tag_length; ++i) {
        strcat(large_html, "a");
    }
    strcat(large_html, closing_tag);

    create_temp_html_file("test_large_single_tag.html", large_html);
    free(large_html);

    // Test split_HTML function with max_len smaller than tag length
    int result = split_HTML(fopen("test_large_single_tag.html", "r"), 1000);
    assert(result == 1 && "Large single tag HTML splitting should succeed");

    // Cleanup: remove the temporary file
    remove("test_large_single_tag.html");
}

// Test case: Splitting HTML with nested tags
void test_split_HTML_nested_tags() {
    const char* html_content = "<html><body><div><p>This is</p><p>nested</p><p>tags.</p></div></body></html>";
    create_temp_html_file("test_nested_tags.html", html_content);

    // Test split_HTML function with max_len smaller than content size
    int result = split_HTML(fopen("test_nested_tags.html", "r"), 30);
    assert(result == 1 && "Nested tags HTML splitting should succeed");

    // Cleanup: remove the temporary file
    remove("test_nested_tags.html");
}

// Test case: Splitting empty HTML file
void test_split_HTML_empty_file() {
    create_temp_html_file("test_empty_file.html", "");

    // Test split_HTML function with an empty file
    int result = split_HTML(fopen("test_empty_file.html", "r"), 100);
    assert(result == 1 && "Empty HTML file splitting should succeed");

    // Cleanup: remove the temporary file
    remove("test_empty_file.html");
}

// Test case: Splitting HTML with no tags, only plain text
void test_split_HTML_plain_text() {
    const char* html_content = "This is plain text without any tags.";
    create_temp_html_file("test_plain_text.html", html_content);

    // Test split_HTML function with max_len larger than content size
    int result = split_HTML(fopen("test_plain_text.html", "r"), 100);
    assert(result == 1 && "Plain text HTML splitting should succeed");

    // Cleanup: remove the temporary file
    remove("test_plain_text.html");
}

int main() {
    // Run updated tests and provide feedback
    test_split_HTML_large_single_tag();
    test_split_HTML_nested_tags();
    test_split_HTML_empty_file();
    test_split_HTML_plain_text();

    printf("All updated tests passed successfully!\n");

    return 0;
}
