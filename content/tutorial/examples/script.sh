for i in */; do
  if [ "$i" != "CVS/" ]; then
      echo $i
      tar cfz ${i%/}.tar.gz `ls -d ${i}* | grep -v "CVS"`
      # Generate the html with the index for the page
      cd $i
      listFiles=`ls | grep -v "CVS" | grep -v "index.htm"`
      cat > index.htm <<EOF
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<html>

 <head>
  <title>Charm++ Tutorial - Index of example 
EOF
      echo ${i%/} >> index.htm
      cat >> index.htm <<EOF
</title>
 </head>

 <body>
  <h1>Charm++ Tutorial - Index of example
EOF
      echo ${i%/} >> index.htm
      cat >> index.htm <<EOF
</h1>
EOF
      echo "  <img src=\"/icons/compressed.gif\" alt=\"[TAR]\"><a href=\"../${i%/}.tar.gz\">Tar file with all the source code</a><br>" >>index.htm
      echo "  <ul>" >>index.htm
      for file in $listFiles; do
          echo "  <li><img src=\"/icons/text.gif\" alt=\"[TXT]\"> <a href=\"$file\" type=\"text/plain\">$file</a>" >>index.htm
      done
      cat >> index.htm <<EOF
  <ul>
 </body>
</html>
EOF
      cd ..
  fi
done
