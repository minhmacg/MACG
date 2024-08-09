function page_name_output
	for file in output*
		echo $file
		set -l file_name (echo $file | sed -e 's/output_//' -e 's/\.csv//')
		echo $file_name
		set -l page_name (awk -F';' -v p="$file_name" 'BEGIN{} $2 == p {print $3} END{}' ../inc/json_list_editted_ver2 )
		echo $page_name
		awk -v p="$page_name" \
			'BEGIN{FS=";";OFS=";"} \
			$1 ~ "2024" {$2=p;print} \
			END{}' $file > output_"$file_name"_1.csv
	end
end

