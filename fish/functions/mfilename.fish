function mfilename
	echo 'mfilename'
	set -g jlist "../json_list_editted_ver2"
end
# TODO rename this function
function rename_inbox_folder
	# from all directory under this directory, get everything that have "inbox/" in name
	# delete : from everyline
	set -l folder_name (lsd -d1 inbox/*)
	
	for f in $folder_name
		# remove digit from each line
		set -l dir (echo $f | sed -e 's/_[[:digit:]]*$//' -e 's/^inbox\///')
		set -l newdir (cat $jlist | grep $dir | cut -d";" -f2)
		# rename
		mv $f inbox/$newdir
	end
end

function cp_json
	#TODO put .json to key (f2)
	#DONE
	
	#TODO edit jsonlist file if anything change
	
	mkdir -p cp
	set -l lines (cat ../json_list_editted_ver2)
	for line in $lines
		set -l v (echo $line | cut -d";" -f2)

		cp ./inbox/$v/message_1.json ./cp/$v.json
	end
end

function cv_json
	cd cp
	for f in (ls *json)
		cat $f | jq . | iconv -f utf8 -t latin1 > output_"$f"
	end

	lsd -l .
	cd ..
	cp cp/output* ../json_src/
end


# TODO: print_to_line recursive

# main
function rncpcv
	rename_inbox_folder
	cp_json
	cv_json
end
