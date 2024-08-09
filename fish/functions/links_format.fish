function links_format
	for files in *txt
	# remove lines contain '[' or ']'; remove all spaces at the beginning of a line; remove all "
	sed -i -e '/^\[\|\]/d' -e 's/^\s*//' -e 's/"//g' $files 
	end
end
