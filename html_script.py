from datetime import date

# this file is for generating the docs from the simple and core html files that come from pdoc3

# apply html formatting


def apply_formatting(dpg_module):
    
    # opening the simple file to scan for where to insert the logo
    with open('html\\dearpygui\\' + dpg_module + '.html', 'r') as f:
        api_html = f.readlines()
    for index, line in enumerate(api_html):
        if line == '<body>\n':
            break

    # inserting the logo and horizontal line
    api_html.insert(index, '<hr>\n')
    logo_html = ['<header>\n', '<h1><a href="index.html">Dear PyGui</a></h1>\n', '</header>\n']
    for count in logo_html:
        api_html.insert(index, logo_html.pop())

    # scan again this time for the place to insert the link back to the main api_docs page
    for index, line in enumerate(api_html):
        if 'title="dearpygui"' in line:
            break

    # removing the old link and inserting the new inserting the new link
    api_html.pop(index)
    api_html.insert(index, '<li><code><a title="dearpygui" href="index.html">dearpygui</a></code></li>\n')

    # writing the final api docs with logo
    with open('api_' + dpg_module + '.html', 'w') as f:
        f.writelines(api_html)
  
      
apply_formatting("dearpygui")
apply_formatting("_dearpygui")
apply_formatting("themes")
apply_formatting("logger")
apply_formatting("demo")
        

# INDEX/HOMEPAGE
# opening the index/homepage file to scan for where to update the date
with open('index.html', 'r') as f:
    index_html = f.readlines()
for index, line in enumerate(index_html):
    if "Last Updated:" in line:
        break

# removing the old link and inserting the new inserting the new link
index_html.pop(index)
index_html.insert(index, f"Last Updated: {date.today()}\n")

# writing the final api docs with logo
with open('index.html', 'w') as f:
    f.writelines(index_html)
