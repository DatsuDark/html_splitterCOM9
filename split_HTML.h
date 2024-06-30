/*Function for splitting an HTML document into fragments.

The first argument is an html file.
The second argument is the maximum possible file fragment size (number of characters).

The function requires the library <String.h>.

If the job completes successfully, the function returns 1.*/


int split_HTML(FILE* html, int max_len)
{
	/*Dictionary for opening tags*/
	const char open[8][9] = {"<p>", "<b>", "<strong>", "<i>", "<ul>", "<ol>", "<div>", "<span>"};
	/*Dictionary for closing tags*/
	const char close[8][10] = { "</p>", "</b>", "</strong>", "</i>", "</ul>", "</ol>", "</div>", "</span>" };
	fseek(html, 0, SEEK_SET);
	char ch;
	/*A buffer into which the contents of the document will be written. 
	When full, the buffer will be written to the html file*/
	char output[4098] = "";
	int index_output = 0;
	int len_output = 0;
	/*Stack for block tags*/
	char stack[100][100];
	char tag[500] = "";
	int index_tag = 0;
	int is_tag_open = 0;
	int len_stack = 0;
	int count_tag_in_stack = 0;
	int index_stack = 0;
	int number_html = 0;
	for (int i = 0; i < 100; i++)
	{
		memset(stack[i], '\0', 100);
	}
	while (1)
	{
		ch = fgetc(html);
		/*If we reach the end of the file, write the last data to the html file*/
		if (ch == -1)
		{
			for (int i = count_tag_in_stack - 1; i >= 0; i--)
			{
				char assist[500] = "";
				strcpy(assist, stack[i]);
				if (assist[1] != '/')
				{
					for (int i = strlen(assist); i > 0; i--)
					{
						assist[i] = assist[i - 1];
					}
					assist[1] = '/';
				}
				strcpy(output + len_output + 1, assist);
				len_output += strlen(assist);
			}
			char name[500] = "";
			snprintf(name, 500, "%s%i.html", "HTML-", number_html);
			FILE* qw = fopen(name, "w");
			fputs(output, qw);
			fclose(qw);
			break;
		}
		/*If we see '<', we start writing the tag to the auxiliary variable*/
		else if (ch == '<')
		{
			is_tag_open = 1;
			tag[index_tag] = ch;
			index_tag++;
		}
		/*If we see '>', we check whether the tag is a block tag and, if necessary, push it onto the stack*/
		else if (ch == '>')
		{
			is_tag_open = 0;
			tag[index_tag] = ch;
			for (int i = 0; i < 8; i++)
			{
				if (strcmp(open[i], tag) == 0)
				{
					strcpy(stack[index_stack], tag);
					len_stack += strlen(tag);
					count_tag_in_stack++;
					index_stack++;
				}
			}
			for (int i = 0; i < 8; i++)
			{
				if (strcmp(close[i], tag) == 0)
				{
					strcpy(stack[index_stack], tag);
					len_stack += strlen(tag);
					count_tag_in_stack++;
					index_stack++;
					for (int j = 0; j < 8; j++)
					{
						if ((strcmp(stack[index_stack - 1], close[j]) == 0) && (strcmp(stack[index_stack - 2], open[j]) == 0))
						{
							memset(stack[index_stack - 2], '\0', 100);
							memset(stack[index_stack - 1], '\0', 100);
							index_stack -= 2;
							len_stack -= (strlen(open[j]) + strlen(close[j]));
							count_tag_in_stack -= 2;
						}
					}
				}
			}
			/*If the tag is not block and there is enough space in the buffer for it, write this tag to the buffer*/
			if ((max_len - len_output) > (strlen(tag) + len_stack))
			{
				strcpy(output + len_output, tag);
				len_output += strlen(tag);
				index_output += strlen(tag);
				memset(tag, '\0', 500);
				index_tag = 0;
			}
			/*If the tag is not block and there is not enough space in the buffer for it, write the contents of the buffer to an html file*/
			if ((max_len - len_output) <= (strlen(tag) + len_stack))
			{
				for (int i = count_tag_in_stack - 1; i >= 0; i--)
				{
					char assist[500] = "";
					strcpy(assist, stack[i]);
					if (assist[1] != '/')
					{
						for (int i = strlen(assist); i > 0; i--)
						{
							assist[i] = assist[i - 1];
						}
						assist[1] = '/';
					}
					strcpy(output + len_output + 1, assist);
					len_output += strlen(assist);
				}
				char name[500] = "";
				snprintf(name, 500, "%s%i.html", "HTML-", number_html);
				FILE* qw = fopen(name, "w");
				fputs(output, qw);
				fclose(qw);
				number_html++;
				memset(output, '\0', 4098);
				index_output = 0;
				len_output = 0;
				if (tag[0] != '\0')
				{
					strcpy(output + len_output, tag);
					len_output += strlen(tag);
					index_output += strlen(tag);
					memset(tag, '\0', 500);
					index_tag = 0;
				}
				for (int i = 0; i < count_tag_in_stack; i++)
				{
					strcpy(output + len_output, stack[i]);
					len_output += strlen(stack[i]);
					index_output += strlen(stack[i]);
				}
			}
		}
		else if (is_tag_open == 1)
		{
			tag[index_tag] = ch;
			index_tag++;
		}
		else if (is_tag_open == 0)
		{
			output[index_output] = ch;
			index_output++;
			len_output++;
			if (max_len - len_output <= len_stack)
			{
				for (int i = count_tag_in_stack - 1; i >= 0; i--)
				{
					char assist[500] = "";
					strcpy(assist, stack[i]);
					if (assist[1] != '/')
					{
						for (int i = strlen(assist); i > 0; i--)
						{
							assist[i] = assist[i - 1];
						}
						assist[1] = '/';
					}
					strcpy(output + len_output, assist);
					len_output += strlen(assist);
				}
				char name[500] = "";
				snprintf(name, 500, "%s%i.html", "HTML-", number_html);
				FILE* qw = fopen(name, "w");
				fputs(output, qw);
				fclose(qw);
				number_html++;
				memset(output, '\0', 4098);
				index_output = 0;
				len_output = 0;
				if (tag[0] != '\0')
				{
					strcpy(output + len_output, tag);
					len_output += strlen(tag);
					index_output += strlen(tag);
					memset(tag, '\0', 500);
					index_tag = 0;
				}
				for (int i = 0; i < count_tag_in_stack; i++)
				{
					strcpy(output + len_output, stack[i]);
					len_output += strlen(stack[i]);
					index_output += strlen(stack[i]);
				}
			}
		}
	}
	return 1;
}