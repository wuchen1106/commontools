#!/bin/bash
#**********************************************read card list****************************************************
card_name="hist2canvas_card.txt"
trans_name="hist2canvas_trans.C.template"
trans_name_new="hist2canvas_trans.C"
file_type=".pdf"
echo "Loading card information from "$card_name
num_marker_list=`gawk 'BEGIN{RS="//hist2canvas_list\n";i=0};{i++};END{print i}' $card_name`
((num_marker_list--))
if [ $num_marker_list != 2 ]; then
  echo "Please check hist2canvas_list in "$card_name
  echo "There should be exactly one pair of this mark, but we have found "$num_marker_list" marks!"
  exit 1
fi
gawk 'BEGIN{FS="//hist2canvas_list\n";RS="You will ont find me, will you?";ORS=""};{print $2}' $card_name > temp_list
declare -a array_canvas
declare -a array_canvas_column
declare -a array_canvas_row
declare -a array_canvas_width
declare -a array_canvas_length
declare -a array_hist
declare -a array_hist_type
declare -a array_hist_file
declare -a array_hist_path
declare -a array_hist_file_name
declare -a array_hist_path_name
declare -a array_hist_canvas
declare -a array_hist_pad
declare -a array_hist_color
declare -a array_file
declare -a array_file_name
num_canvas=0
num_hist=0
num_file=0
num_line=`gawk '{i++};END{print i}' temp_list`
for (( iline=1; iline<=num_line; iline++ ))
do 
  sed -n "$iline s/\(.*\)/\1/p" temp_list > temp
  if grep -E -q '^ *$|^#|^ *//' temp ; then
    rm temp
    continue
  fi
  if grep -q 'canvas:' temp ; then
    array_canvas[num_canvas]=`gawk '{print $2}' temp`
    array_canvas_column[num_canvas]=`gawk '{print $3}' temp`
    array_canvas_row[num_canvas]=`gawk '{print $4}' temp`
    array_canvas_width[num_canvas]=`gawk '{print $5}' temp`
    array_canvas_length[num_canvas]=`gawk '{print $6}' temp`
    ((num_canvas++))
  elif grep -q 'pad:' temp ; then
    pad_index=`gawk '{print $2}' temp`
  else
    array_hist_type[num_hist]=`gawk '{print $1}' temp`
    array_hist_file[num_hist]=`gawk '{print $2}' temp`
    array_hist_path[num_hist]=`gawk '{print $3}' temp`
    array_hist[num_hist]=`gawk '{print $4}' temp`
    array_hist_canvas[num_hist]=${array_canvas[((num_canvas-1))]}
    array_hist_pad[num_hist]=$pad_index
    array_hist_color[num_hist]=`gawk '{print $5}' temp`
    sed -i "s/\//_/g" temp
    sed -i "s/\./_/g" temp
    array_hist_file_name[num_hist]=`gawk '{print $2}' temp`
    array_hist_path_name[num_hist]=`gawk '{print $3}' temp`
    hasThisFileFound="no"
    for (( iter_file=0; iter_file<num_file; iter_file++ ))
    do
      if [ ${array_file[iter_file]} == ${array_hist_file[num_hist]} ]; then
        hasThisFileFound="yes"
        break
      fi
    done
    if [ $hasThisFileFound == "no" ]; then
      array_file[num_file]=${array_hist_file[num_hist]}
      array_file_name[num_file]=${array_hist_file_name[num_hist]}
      echo "new file "${array_hist_file_name[num_hist]}" found!"
      ((num_file++))
    fi
    echo "hist: "${array_hist[num_hist]}
    echo "     type: "${array_hist_type[num_hist]}
    echo "     file: "${array_hist_file[num_hist]}
    echo "     path: "${array_hist_path[num_hist]}
    echo "     file as name: "${array_hist_file_name[num_hist]}
    echo "     path as name: "${array_hist_path_name[num_hist]}
    echo "     canvas: "${array_hist_canvas[num_hist]}
    echo "     pad: "${array_hist_pad[num_hist]}
    echo "     color: "${array_hist_color[num_hist]}
    ((num_hist++))
  fi
  rm temp
done
rm temp_list
#**********************************************write****************************************************
cat $trans_name > $trans_name_new
num_line_trans=`gawk '{i++};END{print i}' $trans_name_new`
for (( iline=1; iline<=num_line_trans; iline++ ))
do 
  sed -n "$iline s/\(.*\)/\1/p" $trans_name_new > temp
  if grep -q '^ *//hist2canvas' temp ; then
    indent_forIline=`sed -n "s/\(.*\)\/\/hist2canvas.*/\1/p" temp`
    touch temp_forIline_insert
#**********************************************write part 1****************************************************
    for (( iter_file=0; iter_file<num_file; iter_file++ ))
    do
      echo "$indent_forIline""TFile* file_"${array_file_name[iter_file]}" = new TFile( \""${array_file[iter_file]}"\" );" >> temp_forIline_insert
      echo "$indent_forIline""if ( file_"${array_file_name[iter_file]}" -> IsZombie() ){" >> temp_forIline_insert
      echo "$indent_forIline""  std::cout<<\"Cannot create file "${array_file[iter_file]}"\"<<std::endl;" >> temp_forIline_insert
      echo "$indent_forIline""  exit(EXIT_FAILURE);" >> temp_forIline_insert
      echo "$indent_forIline""}" >> temp_forIline_insert
    done
    echo "" >> temp_forIline_insert
    echo "$indent_forIline""TObject* myObj;" >> temp_forIline_insert
    echo "$indent_forIline""TFolder* myFolder;" >> temp_forIline_insert
    echo "" >> temp_forIline_insert
    for (( iter_hist=0; iter_hist<num_hist; iter_hist++ ))
    do
      echo "$indent_forIline"${array_hist_type[iter_hist]}"* hist_"${array_hist_file_name[iter_hist]}"_"${array_hist_path_name[iter_hist]}"_"${array_hist[iter_hist]}";" >> temp_forIline_insert
      echo ${array_hist_path[iter_hist]} > temp_forPath
      num_path=`gawk 'BEGIN{RS="/";i=0};{i++};END{print i}' temp_forPath`
      declare -a array_path
      for (( iter_path=1; iter_path<=num_path; iter_path++ ))
      do
        array_path[iter_path]=`gawk -v i=$iter_path 'BEGIN{RS="\n";FS="/";ORS=""};{print $i}' temp_forPath`
        if [ $iter_path == 1 ]; then
          echo "$indent_forIline""myFolder = file2folder( file_"${array_hist_file_name[iter_hist]}", \""${array_path[iter_path]}"\" );" >> temp_forIline_insert
        else
          echo "$indent_forIline""myFolder = folder2folder( myFolder, \""${array_path[iter_path]}"\" );" >> temp_forIline_insert
        fi
      done
      rm temp_forPath
      echo "$indent_forIline""myObj = folder2obj( myFolder, \""${array_hist[iter_hist]}"\" );" >> temp_forIline_insert
      echo "$indent_forIline""if ( ! myObj -> InheritsFrom(\""${array_hist_type[iter_hist]}"\") ){" >> temp_forIline_insert
      echo "$indent_forIline""  std::cout<<\""${array_hist[iter_hist]}" in "${array_hist_file[iter_hist]}" is not a "${array_hist_type[iter_hist]}"!\"<<std::endl;" >> temp_forIline_insert
      echo "$indent_forIline""  exit(EXIT_FAILURE);" >> temp_forIline_insert
      echo "$indent_forIline""}" >> temp_forIline_insert
      echo "$indent_forIline""hist_"${array_hist_file_name[iter_hist]}"_"${array_hist_path_name[iter_hist]}"_"${array_hist[iter_hist]}" = dynamic_cast<"${array_hist_type[iter_hist]}"*> ( myObj );" >> temp_forIline_insert
      echo "$indent_forIline""if ( hist_"${array_hist_file_name[iter_hist]}"_"${array_hist_path_name[iter_hist]}"_"${array_hist[iter_hist]}" == NULL ){" >> temp_forIline_insert
      echo "$indent_forIline""  std::cout<<\"Cannot convert "${array_hist[iter_hist]}" in "${array_hist_file[iter_hist]}" to "${array_hist_type[iter_hist]}"!\"<<std::endl;" >> temp_forIline_insert
      echo "$indent_forIline""  exit(EXIT_FAILURE);" >> temp_forIline_insert
      echo "$indent_forIline""}" >> temp_forIline_insert
    done
#**********************************************write part 2****************************************************
    for (( iter_canvas=0; iter_canvas<num_canvas; iter_canvas++ ))
    do
      echo "" >> temp_forIline_insert
      echo "$indent_forIline""TCanvas "${array_canvas[iter_canvas]}"( \""${array_canvas[iter_canvas]}"\", \""${array_canvas[iter_canvas]}"\", "${array_canvas_width[iter_canvas]}", "${array_canvas_length[iter_canvas]}" );" >> temp_forIline_insert
      echo "$indent_forIline"${array_canvas[iter_canvas]}".Divide( "${array_canvas_column[iter_canvas]}", "${array_canvas_row[iter_canvas]}" );" >> temp_forIline_insert
      num_column=${array_canvas_column[iter_canvas]}
      num_row=${array_canvas_row[iter_canvas]}
      ((num_pad=num_column*num_row))
      for (( iter_pad=1; iter_pad<=num_pad; iter_pad++ ))
      do
        hasDrawn="no"
        echo "$indent_forIline"${array_canvas[iter_canvas]}".cd( "$iter_pad" );" >> temp_forIline_insert
        for (( iter_hist=0; iter_hist<num_hist; iter_hist++ ))
        do
          if [ ${array_hist_canvas[iter_hist]} == ${array_canvas[iter_canvas]} -a ${array_hist_pad[iter_hist]} == $iter_pad ]; then
            echo "$indent_forIline""hist_"${array_hist_file_name[iter_hist]}"_"${array_hist_path_name[iter_hist]}"_"${array_hist[iter_hist]}" -> SetLineColor( k"${array_hist_color[iter_hist]}" );" >> temp_forIline_insert
            if [ $hasDrawn == "no" ]; then
              echo "$indent_forIline""hist_"${array_hist_file_name[iter_hist]}"_"${array_hist_path_name[iter_hist]}"_"${array_hist[iter_hist]}" -> Draw();" >> temp_forIline_insert
              hasDrawn="yes"
            else
              echo "$indent_forIline""hist_"${array_hist_file_name[iter_hist]}"_"${array_hist_path_name[iter_hist]}"_"${array_hist[iter_hist]}" -> Draw( \"Same\" );" >> temp_forIline_insert
            fi
          fi
        done
      done
      echo "$indent_forIline"${array_canvas[iter_canvas]}".Print( \""${array_canvas[iter_canvas]}$file_type"\" );" >> temp_forIline_insert
    done
#modify target file
    gawk -v gawk_iline=$iline 'BEGIN{RS="\n";ORS="\n";i=1};{if(i<gawk_iline)print;i++}' $trans_name_new > temp_forIline_1
    gawk -v gawk_iline=$iline 'BEGIN{RS="\n";ORS="\n";i=1};{if(i>gawk_iline)print;i++}' $trans_name_new > temp_forIline_2
    gawk '//' temp_forIline_1 temp_forIline_insert temp_forIline_2 > $trans_name_new
    num_line_temp_forIline_insert=`gawk '{i++};END{print i}' temp_forIline_insert`
    (( num_line_trans=num_line_trans+num_line_temp_forIline_insert-1 ))
    (( iline=iline+num_line_temp_forIline_insert-1 ))
    rm temp_forIline_insert
    rm temp_forIline_1
    rm temp_forIline_2
  fi
  rm temp
done
