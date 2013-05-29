#!/bin/bash

#*********************************read the card *********************************************

card_name="tuple2hist_card.txt"

num_line=0
num_tree=0
num_branch=0
num_func=0
num_cut=0
num_1D=0
num_2D=0

declare -a array_tree
declare -a array_branch
declare -a array_branch_belong
declare -a array_branch_type
declare -a array_1D_DOCUT
declare -a array_1D_CUT
declare -a array_1D_variable
declare -a array_1D_tree
declare -a array_1D_type
declare -a array_1D_hist_type
declare -a array_1D_bin
declare -a array_1D_left
declare -a array_1D_right
declare -a array_2D_DOCUT
declare -a array_2D_CUT
declare -a array_2D_tree
declare -a array_2D_hist_type
declare -a array_2D_variable_X
declare -a array_2D_type_X
declare -a array_2D_bin_X
declare -a array_2D_left_X
declare -a array_2D_right_X
declare -a array_2D_variable_Y
declare -a array_2D_type_Y
declare -a array_2D_bin_Y
declare -a array_2D_left_Y
declare -a array_2D_right_Y
declare -a array_func
declare -a array_func_type
declare -a array_func_belong
declare -a array_cut
declare -a array_cut_belong

Filename_found=0
num_line=`gawk '{i++};END{print i}' $card_name`

for (( iline=1; iline<=num_line; iline++ ))
do 

  sed -n "$iline s/\(.*\)/\1/p" $card_name > temp

#ignore null line, line start with # or //
  if grep -E -q '^ *$|^#|^//' temp ; then
    $RM temp
    continue
  fi

#get Filename
  if grep -q 'Filename:' temp ; then
    filename=`gawk '{print $2}' temp`
    Filename_found=1
    $RM temp
    continue
  fi
  if [ $Filename_found == 0 ]; then
    echo "You should put Filename in the very beginning!!!"
    $RM temp
    exit 1
  fi

#get tuple
  if grep -q 'Tuplename:' temp ; then
    tree_declared=0
    tree_name=`gawk '{print $2}' temp`
    for (( iter_tree=0; iter_tree<num_tree; iter_tree++ ))
    do
      if [ ${array_tree[iter_tree]} == $tree_name ]; then
        tree_declared=1
        break
      fi
    done
    if [ $tree_declared == 1 ]; then
      echo "Please check line "$iline"!!!"
      echo "There is a TTree named "$tree_name" in the fore part!!!"
      $RM temp
      exit 1
    fi
    array_tree[((num_tree++))]=`gawk '{print $2}' temp`
    echo TTree ${array_tree[((num_tree-1))]}
    $RM temp
    continue
  fi

#get branch
  if [ $num_tree == 0 ]; then
    echo "Please check line "$iline"!!!"
    echo "You should put Tuplename before its branch!!!"
    $RM temp
    exit 1
  fi
  if ! grep -q 'tuple2hist_FUNC' temp ; then
    if grep -E -q '^TH1:|TH2_[Xx]:|TH2_[Yy]:' temp ;then
      branch_declared=0
      branch_name=`gawk '{print $2}' temp`
      for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
      do
        if [ ${array_branch_belong[iter_branch]} == ${array_tree[((num_tree-1))]} ]; then
          if [ ${array_branch[iter_branch]} == $branch_name ]; then
            branch_declared=1
            break
          fi
        fi
      done
      if [ $branch_declared == 0 ]; then
        array_branch[num_branch]=`gawk '{print $2}' temp`
        array_branch_type[num_branch]=`gawk '{print $4}' temp`
        array_branch_belong[((num_branch++))]=${array_tree[((num_tree-1))]}
      fi
    fi
  fi

#get cut
  if grep -q 'tuple2hist_CUT' temp ; then
    array_cut[num_cut]=`sed 's/.*tuple2hist_CUT_\(\w*\)\>.*/\1/g' temp`
    array_cut_belong[num_cut]=${array_tree[((num_tree-1))]}

    cut_declared=0
    cut_name=`sed 's/.*tuple2hist_CUT_\(\w*\)\>.*/\1/g' temp`
    for (( iter_cut=0; iter_cut<num_cut; iter_cut++ ))
    do
      if [ ${array_cut_belong[iter_cut]} == ${array_tree[((num_tree-1))]} ]; then
        if [ ${array_cut[iter_cut]} == $cut_name ]; then
          cut_declared=1
          break
        fi
      fi
    done
    if [ $cut_declared == 0 ]; then
      keyword="//CUT_"${array_cut[num_cut]}"\n"
      num_marker_cut=`gawk -v "gawk_keyword=$keyword" 'BEGIN{RS=gawk_keyword;i=0};{i++};END{print i}' $card_name`
      ((num_marker_cut--))
      if [ $num_marker_cut != 3 ]; then
        echo "Please check "$keyword" in "$card_name
        echo "There should be exactly three of this mark, but we have found "$num_marker_cut" marks!"
        $RM temp
        exit 1
      fi

      gawk -v "gawk_keyword=$keyword" 'BEGIN{FS=gawk_keyword;RS="You will not find me, will you?";ORS=""};{print $2}' $card_name > temp_cut_branch
      gawk -v "gawk_keyword=$keyword" 'BEGIN{FS=gawk_keyword;RS="You will not find me, will you?";ORS=""};{print $3}' $card_name > temp_cut_critera_${array_cut[iter_cut]}
      sed -i "s/\(.*\)/    if( \1 ){/g" temp_cut_critera_${array_cut[iter_cut]}

      num_line_temp_cut_branch=`gawk '{i++};END{print i}' temp_cut_branch`

      for (( ilne_temp_cut_branch=1; ilne_temp_cut_branch<=num_line_temp_cut_branch; ilne_temp_cut_branch++ ))
      do 

        sed -n "$ilne_temp_cut_branch s/\(.*\)/\1/p" temp_cut_branch > temp_line_cut_branch

        if grep -E -q '^ *$|^#|^//' temp_line_cut_branch ; then
          $RM temp_line_cut_branch
          continue
        fi
        branch_declared=0
        branch_name=`gawk '{print $1}' temp_line_cut_branch`
        for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
        do
          if [ ${array_branch_belong[iter_branch]} == ${array_tree[((num_tree-1))]} ]; then
            if [ ${array_branch[iter_branch]} == $branch_name ]; then
              branch_declared=1
              break
            fi
          fi
        done
        if [ $branch_declared == 0 ]; then
          array_branch[num_branch]=`gawk '{print $1}' temp_line_cut_branch`
          array_branch_type[num_branch]=`gawk '{print $2}' temp_line_cut_branch`
          array_branch_belong[((num_branch++))]=${array_tree[((num_tree-1))]}
        fi
        $RM temp_line_cut_branch
      done
      $RM temp_cut_branch
      ((num_cut++))
    fi
  fi

#get func
  if grep -q 'tuple2hist_FUNC' temp ; then
    array_func[num_func]=`gawk '{print $2}' temp`
    array_func_type[num_func]=`gawk '{print $4}' temp`
    array_func_belong[num_func]=${array_tree[((num_tree-1))]}

    func_declared=0
    func_name=`gawk '{print $2}' temp`
    for (( iter_func=0; iter_func<num_func; iter_func++ ))
    do
      if [ ${array_func_belong[iter_func]} == ${array_tree[((num_tree-1))]} ]; then
        if [ ${array_func[iter_func]} == $func_name ]; then
          func_declared=1
          break
        fi
      fi
    done
    if [ $func_declared == 0 ]; then
      keyword="//"${array_func[num_func]}"\n"
      num_marker_func=`gawk -v "gawk_keyword=$keyword" 'BEGIN{RS=gawk_keyword;i=0};{i++};END{print i}' $card_name`
      ((num_marker_func--))
      if [ $num_marker_func != 3 ]; then
        echo "Please check "$keyword" in "$card_name
        echo "There should be exactly three of this mark, but we have found "$num_marker_func" marks!"
        $RM temp
        exit 1
      fi

      gawk -v "gawk_keyword=$keyword" 'BEGIN{FS=gawk_keyword;RS="You will not find me, will you?";ORS=""};{print $2}' $card_name > temp_func_branch
      gawk -v "gawk_keyword=$keyword" 'BEGIN{FS=gawk_keyword;RS="You will not find me, will you?";ORS=""};{print $3}' $card_name > temp_func_calculate_${array_func[iter_func]}

      num_line_temp_func_branch=`gawk '{i++};END{print i}' temp_func_branch`

      for (( ilne_temp_func_branch=1; ilne_temp_func_branch<=num_line_temp_func_branch; ilne_temp_func_branch++ ))
      do 

        sed -n "$ilne_temp_func_branch s/\(.*\)/\1/p" temp_func_branch > temp_line_func_branch

        if grep -E -q '^ *$|^#|^//' temp_line_func_branch ; then
          $RM temp_line_func_branch
          continue
        fi
        branch_declared=0
        branch_name=`gawk '{print $1}' temp_line_func_branch`
        for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
        do
          if [ ${array_branch_belong[iter_branch]} == ${array_tree[((num_tree-1))]} ]; then
            if [ ${array_branch[iter_branch]} == $branch_name ]; then
              branch_declared=1
              break
            fi
          fi
        done
        if [ $branch_declared == 0 ]; then
          array_branch[num_branch]=`gawk '{print $1}' temp_line_func_branch`
          array_branch_type[num_branch]=`gawk '{print $2}' temp_line_func_branch`
          array_branch_belong[((num_branch++))]=${array_tree[((num_tree-1))]}
        fi
        $RM temp_line_func_branch
      done
      $RM temp_func_branch
      ((num_func++))
    fi
  fi

#get TH2
  if grep -q 'TH2_[Xx]:' temp; then
    array_2D_tree[num_2D]=${array_tree[((num_tree-1))]}
    branch_X_name=`gawk '{print $2}' temp`
    array_2D_variable_X[num_2D]=`gawk '{print $2}' temp`
    array_2D_hist_type[num_2D]=`gawk '{print $3}' temp`
    array_2D_type_X[num_2D]=`gawk '{print $4}' temp`
    array_2D_bin_X[num_2D]=`gawk '{print $5}' temp`
    array_2D_left_X[num_2D]=`gawk '{print $6}' temp`
    array_2D_right_X[num_2D]=`gawk '{print $7}' temp`
    iline_next=$iline
    ((iline_next++))
    sed -n "$iline_next s/\(.*\)/\1/p" $card_name > temp_next
    if ! grep -q 'TH2_[Yy]:' temp_next; then
      echo "  Syntax error: missing TH2_Y!!!"
      $RM temp
      $RM temp_next
      exit 1
    fi
    branch_Y_name=`gawk '{print $2}' temp_next`
    hist_2D_declared=0
    for (( iter_2D=0; iter_2D<num_2D; iter_2D++ ))
    do
      if [ ${array_2D_tree[iter_2D]} == ${array_tree[((num_tree-1))]} ]; then
        if [ ${array_2D_variable_X[iter_2D]} == $branch_X_name ]; then
          if [ ${array_2D_variable_Y[iter_2D]} == $branch_Y_name ]; then
            echo "Please check line "$iline" !!!"
            echo "There is a TH2 same with this one in the fore part!!!"
            hist_2D_declared=1
            break
          fi
        fi
      fi
    done
    if [ $hist_2D_declared == 1 ]; then
      echo "Program chooses to ignore this TH2"
      $RM temp
      $RM temp_next
      continue
    fi
    array_2D_variable_Y[num_2D]=`gawk '{print $2}' temp_next`
    if [ ${array_2D_hist_type[num_2D]} == "TH1D" ]; then
      echo " wrong hist_type \""${array_2D_hist_type[num_2D]}"\" for 2D_hist "${array_2D_tree[num_2D]}"_"${array_2D_variable_X[num_2D]}"_"${array_2D_variable_Y[num_2D]}
      $RM temp
      $RM temp_next
      exit 1
    fi
    if [ ${array_2D_hist_type[num_2D]} == "TH1F" ]; then
      echo " wrong hist_type \""${array_2D_hist_type[num_2D]}"\" for 2D_hist "${array_2D_tree[num_2D]}"_"${array_2D_variable_X[num_2D]}"_"${array_2D_variable_Y[num_2D]}
      $RM temp
      $RM temp_next
      exit 1
    fi
    if [ ${array_2D_hist_type[num_2D]} == "TH1T" ]; then
      echo " wrong hist_type \""${array_2D_hist_type[num_2D]}"\" for 2D_hist "${array_2D_tree[num_2D]}"_"${array_2D_variable_X[num_2D]}"_"${array_2D_variable_Y[num_2D]}
      $RM temp
      $RM temp_next
      exit 1
    fi
    if [ ${array_2D_hist_type[num_2D]} == "TH1" ]; then
      echo " wrong hist_type \""${array_2D_hist_type[num_2D]}"\" for 2D_hist "${array_2D_tree[num_2D]}"_"${array_2D_variable_X[num_2D]}"_"${array_2D_variable_Y[num_2D]}
      $RM temp
      $RM temp_next
      exit 1
    fi
    array_2D_type_Y[num_2D]=`gawk '{print $4}' temp`
    array_2D_bin_Y[num_2D]=`gawk '{print $5}' temp_next`
    array_2D_left_Y[num_2D]=`gawk '{print $6}' temp_next`
    array_2D_right_Y[num_2D]=`gawk '{print $7}' temp_next`
    echo "     "${array_2D_hist_type[num_2D]}" : "
    echo "          tree            "${array_tree[num_tree-1]}
    echo "          variable_X      "${array_2D_variable_X[num_2D]}
    echo "          variable_X_type "${array_2D_type_X[num_2D]}
    echo "          bin_X           "${array_2D_bin_X[num_2D]}
    echo "          left_X          "${array_2D_left_X[num_2D]}
    echo "          right_X         "${array_2D_right_X[num_2D]}
    echo "          variable_Y      "${array_2D_variable_Y[num_2D]}
    echo "          vairable_Y_type "${array_2D_type_Y[num_2D]}
    echo "          bin_Y           "${array_2D_bin_Y[num_2D]}
    echo "          left_Y          "${array_2D_left_Y[num_2D]}
    echo "          right_Y         "${array_2D_right_Y[num_2D]}
    if grep -q 'tuple2hist_CUT' temp; then
      array_2D_DOCUT[num_2D]=Y
      array_2D_CUT[num_2D]=`sed 's/.*tuple2hist_CUT_\(\w*\)\>.*/\1/g' temp`
      echo "          with cut critera:"${array_2D_CUT[num_2D]}
    else
      array_2D_DOCUT[num_2D]=N
      echo "          with no cut"
    fi
    ((num_2D++))
    $RM temp
    $RM temp_next
    continue
  fi
  if grep -q 'TH2_[Yy]:' temp; then
    $RM temp
    continue
  fi

#get TH1
  if grep -q 'TH1:' temp; then
    hist_1D_declared=0
    branch_name=`gawk '{print $2}' temp`
    for (( iter_1D=0; iter_1D<num_1D; iter_1D++ ))
    do
      if [ ${array_1D_tree[iter_1D]} == ${array_tree[((num_tree-1))]} ]; then
        if [ ${array_1D_variable[iter_1D]} == $branch_name ]; then
          echo "Please check line "$iline" !!!"
          echo "There is a TH1 same with this one in the fore part!!!"
          hist_1D_declared=1
          break
        fi
      fi
    done
    if [ $hist_1D_declared == 1 ]; then
      echo "Program chooses to ignore this TH1"
      $RM temp
      continue
    fi
    array_1D_tree[num_1D]=${array_tree[((num_tree-1))]}
    array_1D_variable[num_1D]=`gawk '{print $2}' temp`
    array_1D_hist_type[num_1D]=`gawk '{print $3}' temp`
    array_1D_type[num_1D]=`gawk '{print $4}' temp`
    if [ ${array_1D_hist_type[num_1D]} == "TH1" ]; then
      echo " wrong hist_type \""${array_1D_hist_type[num_1D]}"\" for TBranch "${array_1D_variable[num_1D]}
      $RM temp
      exit 1
    fi
    array_1D_bin[num_1D]=`gawk '{print $5}' temp`
    array_1D_left[num_1D]=`gawk '{print $6}' temp`
    array_1D_right[num_1D]=`gawk '{print $7}' temp`
    echo "     "${array_1D_hist_type[num_1D]}":"
    echo "          tree      " ${array_1D_tree[num_1D]}
    echo "          variable  " ${array_1D_variable[num_1D]}
    echo "          type      " ${array_1D_type[num_1D]}
    echo "          hist_type " ${array_1D_hist_type[num_1D]}
    echo "          bin       " ${array_1D_bin[num_1D]}
    echo "          left      " ${array_1D_left[num_1D]}
    echo "          right     " ${array_1D_right[num_1D]}
    if grep -q 'tuple2hist_CUT' temp; then
      array_1D_DOCUT[num_1D]=Y
      array_1D_CUT[num_1D]=`sed 's/.*tuple2hist_CUT_\(\w*\)\>.*/\1/g' temp`
      echo "          with cut critera:"${array_1D_CUT[num_1D]}
    else
      array_1D_DOCUT[num_1D]=N
      echo "          with no cut"
    fi
    ((num_1D++))
    $RM temp
  fi
done

for (( iter_func=0; iter_func<num_func; iter_func++ ))
do
  for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
  do
    if [ ${array_branch_belong[iter_branch]} == ${array_func_belong[iter_func]} ]; then
      sed -i "s/\<${array_branch[iter_branch]}\>/${array_branch_belong[iter_branch]}_${array_branch[iter_branch]}/g" temp_func_calculate_${array_func[iter_func]}
    fi
  done
  for (( iter_func_2=0; iter_func_2<num_func; iter_func_2++ ))
  do
    if [ ${array_func_belong[iter_func2]} == ${array_func_belong[iter_func]} ]; then
      sed -i "s/\<${array_func[iter_func_2]}\>/${array_func_belong[iter_func_2]}_${array_func[iter_func_2]}/g" temp_func_calculate_${array_func[iter_func]}
    fi
  done
done

for (( iter_cut=0; iter_cut<num_cut; iter_cut++ ))
do
  for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
  do
    if [ ${array_branch_belong[iter_branch]} == ${array_cut_belong[iter_cut]} ]; then
      sed -i "s/\<${array_branch[iter_branch]}\>/${array_branch_belong[iter_branch]}_${array_branch[iter_branch]}/g" temp_cut_critera_${array_cut[iter_cut]}
    fi
  done
done

#***********************************write the first part of code************************************************

echo "//Ignore me! I am just a separator!" > temp1
echo "" >> temp1
echo "  string fileTuple_name = \""$filename"\";" >> temp1
echo "  string fileHist_name = \"hist.root\";" >> temp1
echo "" >> temp1
for (( iter_tree=0; iter_tree<num_tree; iter_tree++ ))
do
  echo "  vec_tree.push_back( \""${array_tree[iter_tree]}"\" );" >> temp1
  for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
  do
    if [ ${array_branch_belong[iter_branch]} == ${array_tree[iter_tree]} ]; then
      echo "  pair<string,string> pair_"${array_tree[iter_tree]}"_"${array_branch[iter_branch]}"( \""${array_tree[iter_tree]}"\", \""${array_branch[iter_branch]}"\" );" >> temp1
      echo "  vec_branch_tree.push_back( pair_"${array_tree[iter_tree]}"_"${array_branch[iter_branch]}" );" >> temp1
    fi
  done
done
echo "" >> temp1
echo "//Ignore me! I am just a separator!" >> temp1

#***********************************write the second part of code***********************************************

echo "//Ignore me! I am just a separator!" > temp2
echo "" >> temp2
for (( iter_tree=0; iter_tree<num_tree; iter_tree++ ))
do
  echo "  TChain* chain_"${array_tree[iter_tree]}" = new TChain( \""${array_tree[iter_tree]}"\" );" >> temp2
done
echo "" >> temp2
for (( iter_tree=0; iter_tree<num_tree; iter_tree++ ))
do
  echo "  chain_"${array_tree[iter_tree]}" -> Add( fileTuple_name.c_str() );" >> temp2
done
echo "" >> temp2
for (( iter_tree=0; iter_tree<num_tree; iter_tree++ ))
do
  echo "  TFolder* folder_"${array_tree[iter_tree]}" = new TFolder( \""${array_tree[iter_tree]}"\", \""${array_tree[iter_tree]}"\" );" >> temp2
done
echo "" >> temp2
for (( iter_tree=0; iter_tree<num_tree; iter_tree++ ))
do
  echo "  list_hist -> Add( folder_"${array_tree[iter_tree]}" );" >> temp2
done
for (( iter_tree=0; iter_tree<num_tree; iter_tree++ ))
do
  blank_line_printed=0
  for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
  do
    if [ ${array_branch_belong[iter_branch]} == ${array_tree[iter_tree]} ]; then
      if [ $blank_line_printed == 0 ]; then
        echo "" >> temp2
        blank_line_printed=1
      fi
      echo "  "${array_branch_type[iter_branch]}" "${array_tree[iter_tree]}"_"${array_branch[iter_branch]}";" >> temp2
    fi
  done
  blank_line_printed=0
  for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
  do
    if [ ${array_branch_belong[iter_branch]} == ${array_tree[iter_tree]} ]; then
      if [ $blank_line_printed == 0 ]; then
        echo "" >> temp2
        blank_line_printed=1
      fi
      echo "  chain_"${array_tree[iter_tree]}" -> SetBranchAddress( \""${array_branch[iter_branch]}"\", &"${array_tree[iter_tree]}"_"${array_branch[iter_branch]}" );" >> temp2
    fi
  done
  blank_line_printed=0
  for (( iter_1D=0; iter_1D<num_1D; iter_1D++ ))
  do
    if [ ${array_1D_tree[iter_1D]} == ${array_tree[iter_tree]} ]; then
      if [ $blank_line_printed == 0 ]; then
        echo "" >> temp2
        blank_line_printed=1
      fi
      echo "  "${array_1D_hist_type[iter_1D]}"* hist_"${array_tree[iter_tree]}"_"${array_1D_variable[iter_1D]}" = new "${array_1D_hist_type[iter_1D]}"( \""${array_tree[iter_tree]}"_"${array_1D_variable[iter_1D]}"\", \""${array_1D_variable[iter_1D]}"\", "${array_1D_bin[iter_1D]}", "${array_1D_left[iter_1D]}", "${array_1D_right[iter_1D]}" );" >> temp2
    fi
  done
  for (( iter_2D=0; iter_2D<num_2D; iter_2D++ ))
  do
    if [ ${array_2D_tree[iter_2D]} == ${array_tree[iter_tree]} ]; then
      echo "  "${array_2D_hist_type[iter_2D]}"* hist_"${array_tree[iter_tree]}"_"${array_2D_variable_X[iter_2D]}"_"${array_2D_variable_Y[iter_2D]}" = new "${array_2D_hist_type[iter_2D]}"( \""${array_tree[iter_tree]}"_"${array_2D_variable_X[iter_2D]}"_"${array_2D_variable_Y[iter_2D]}"\", \""${array_2D_variable_X[iter_2D]}"_"${array_2D_variable_Y[iter_2D]}"\", "${array_2D_bin_X[iter_2D]}", "${array_2D_left_X[iter_2D]}", "${array_2D_right_X[iter_2D]}", "${array_2D_bin_Y[iter_2D]}", "${array_2D_left_Y[iter_2D]}", "${array_2D_right_Y[iter_2D]}" );" >> temp2
    fi
  done
  blank_line_printed=0
  for (( iter_1D=0; iter_1D<num_1D; iter_1D++ ))
  do
    if [ ${array_1D_tree[iter_1D]} == ${array_tree[iter_tree]} ]; then
      if [ $blank_line_printed == 0 ]; then
        echo "" >> temp2
        blank_line_printed=1
      fi
      echo "  folder_"${array_tree[iter_tree]}" -> Add( hist_"${array_tree[iter_tree]}"_"${array_1D_variable[iter_1D]}" );" >> temp2
    fi
  done
  for (( iter_2D=0; iter_2D<num_2D; iter_2D++ ))
  do
    if [ ${array_2D_tree[iter_2D]} == ${array_tree[iter_tree]} ]; then
      echo "  folder_"${array_tree[iter_tree]}" -> Add( hist_"${array_tree[iter_tree]}"_"${array_2D_variable_X[iter_2D]}"_"${array_2D_variable_Y[iter_2D]}" );" >> temp2
    fi
  done
  echo "" >> temp2
  echo "  Long64_t nEvent_"${array_tree[iter_tree]}" = chain_"${array_tree[iter_tree]}" -> GetEntries();" >> temp2
  echo "  for ( Long64_t iEvent = 0; iEvent < nEvent_"${array_tree[iter_tree]}"; iEvent++ ){" >> temp2
  echo "" >> temp2
  echo "    chain_"${array_tree[iter_tree]}" -> GetEntry( iEvent );" >> temp2
  blank_line_printed=0
  for (( iter_func=0; iter_func<num_func; iter_func++ ))
  do
    if [ ${array_func_belong[iter_func]} == ${array_tree[iter_tree]} ]; then
      if [ $blank_line_printed == 0 ]; then
        echo "" >> temp2
        blank_line_printed=1
      fi
      echo "    "${array_func_type[iter_func]}" "${array_func_belong[iter_func]}"_"${array_func[iter_func]}";" >> temp2
      sed -i "s/\(.*\)/    \1/g" temp_func_calculate_${array_func[iter_func]}
      cat temp_func_calculate_${array_func[iter_func]} >> temp2
    fi
  done
  blank_line_printed=0
  for (( iter_1D=0; iter_1D<num_1D; iter_1D++ ))
  do
    if [ ${array_1D_tree[iter_1D]} == ${array_tree[iter_tree]} ]; then
      if [ $blank_line_printed == 0 ]; then
        echo "" >> temp2
        blank_line_printed=1
      fi
      if [ ${array_1D_DOCUT[iter_1D]} == N ]; then
        echo "    hist_"${array_tree[iter_tree]}"_"${array_1D_variable[iter_1D]}" -> Fill( "${array_tree[iter_tree]}"_"${array_1D_variable[iter_1D]}" );" >> temp2
      else
        cat temp_cut_critera_${array_1D_CUT[iter_1D]} >> temp2
        echo "      hist_"${array_tree[iter_tree]}"_"${array_1D_variable[iter_1D]}" -> Fill( "${array_tree[iter_tree]}"_"${array_1D_variable[iter_1D]}" );" >> temp2
        echo "    }" >> temp2
      fi
    fi
  done
  for (( iter_2D=0; iter_2D<num_2D; iter_2D++ ))
  do
    if [ ${array_2D_tree[iter_2D]} == ${array_tree[iter_tree]} ]; then
      if [ ${array_2D_DOCUT[iter_2D]} == N ]; then
        echo "    hist_"${array_tree[iter_tree]}"_"${array_2D_variable_X[iter_2D]}"_"${array_2D_variable_Y[iter_2D]}" -> Fill( "${array_tree[iter_tree]}"_"${array_2D_variable_X[iter_2D]}", "${array_tree[iter_tree]}"_"${array_2D_variable_Y[iter_2D]}" );" >> temp2
      else
        cat temp_cut_critera_${array_2D_CUT[iter_2D]} >> temp2
        echo "      hist_"${array_tree[iter_tree]}"_"${array_2D_variable_X[iter_2D]}"_"${array_2D_variable_Y[iter_2D]}" -> Fill( "${array_tree[iter_tree]}"_"${array_2D_variable_X[iter_2D]}", "${array_tree[iter_tree]}"_"${array_2D_variable_Y[iter_2D]}" );" >> temp2
        echo "    }" >> temp2
      fi
    fi
  done
  echo "" >> temp2
  echo "  }" >> temp2
done
echo "//Ignore me! I am just a separator!" >> temp2

#**************************************put them together*******************************************************

gawk 'BEGIN{FS="//Ignore me! I am just a separator!\n";RS="You will ont find me, will you?";ORS=""};{print $1}' tuple2hist_trans.C > template.1
gawk 'BEGIN{FS="//Ignore me! I am just a separator!\n";RS="You will not find me, will you?";ORS=""};{print $3}' tuple2hist_trans.C > template.2
gawk 'BEGIN{FS="//Ignore me! I am just a separator!\n";RS="You will ont find me, will you?";ORS=""};{print $5}' tuple2hist_trans.C > template.3

gawk '//' template.1 temp1 template.2 temp2 template.3 > tuple2hist_trans.C

$RM template.1
$RM template.2
$RM template.3
$RM temp1
$RM temp2

for (( iter_func=0; iter_func<num_func; iter_func++ ))
do
  $RM temp_func_calculate_${array_func[iter_func]}
done
