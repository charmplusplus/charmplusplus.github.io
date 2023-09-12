# The charmplusplus.org website

See also https://wiki.illinois.edu/wiki/display/ppl/Deploy+the+charmplusplus+website.

This is for deploying the charmplusplus website (http://charmplusplus.org/) which is hosted on the machine "charm". 


##  Building the website
Steps to obtain a local copy of the website for updating:

- Check out the source code:

      $ git clone https://github.com/UIUC-PPL/charmplusplus.org
  
- Install the required gems:

      $ gem install nanoc adsf

- Compile the website (anytime you make changes): 

      $ cd charmweb
      $ var/lib/gems/1.8/bin/nanoc compile
    
Note that the directory where the nanoc executable is stored may be different than this.

- Start a local server (port will print to screeen):

      $ /var/lib/gems/1.8/bin/nanoc view
    
From here you can access the website by accessing http://localhost:PORT/.

## Editing the content

- Change directory to `content`
- Edit the html
- Recompile the website to generate the new set of static pages

## Deploy

Copy the contents of the "output" folder to charm.cs.illinois.edu:/web/charmweb.
