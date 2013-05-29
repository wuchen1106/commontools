#!/bin/bash
#**********************************************preset****************************************************
algorithm_name="ValidDkpi"
algorithm_path_var=`echo $algorithm_name|tr a-z A-Z`'ALGROOT'
algorithm_path=${!algorithm_path_var}
card_name=$algorithm_path"/auto_gen_card.txt"
head_name=$algorithm_path"/"$algorithm_name"Alg/"$algorithm_name".h.template"
head_name_new=$algorithm_path"/"$algorithm_name"Alg/"$algorithm_name".h"
source_name=$algorithm_path"/src/"$algorithm_name".cxx.template"
source_name_new=$algorithm_path"/src/"$algorithm_name".cxx"
manager_svc_name="auto_gen_manager_svc.txt"
echo "Loading card information from "$card_name
num_marker_preset=`gawk 'BEGIN{RS="//auto_gen_preset\n";i=0};{i++};END{print i}' $card_name`
((num_marker_preset--))
if [ $num_marker_preset != 2 ]; then
  echo "Please check auto_gen_preset in "$card_name
  echo "There should be exactly one pair of this mark, but we have found "$num_marker_preset" marks!"
  exit 1
fi
gawk 'BEGIN{FS="//auto_gen_preset\n";RS="You will ont find me, will you?";ORS=""};{print $2}' $card_name > temp_preset
switch_func_1_tuple=0
switch_func_2_svc=0
switch_func_1_tuple_found=0
switch_func_2_svc_found=0
num_line=`gawk '{i++};END{print i}' temp_preset`
for (( iline=1; iline<=num_line; iline++ ))
do 
  sed -n "$iline s/\(.*\)/\1/p" temp_preset> temp
  if grep -E -q '^ *$|^#|^ *//' temp ; then
    rm temp
    continue
  fi
  if grep -q 'func_1_tuple:' temp ; then
    if [ $switch_func_2_svc_found == 0 ]; then
      switch_func_1_tuple_found=1
      if grep -q '[Yy][Ee][Ss]' temp ; then
        switch_func_1_tuple=1
      fi
      rm temp
      continue
    fi
    if [ $switch_func_2_svc_found != 0 ]; then
      echo "Please check line "$iline
      echo "func_1_tuple has been set in the fore part"
      rm temp
      rm temp_preset
      exit 1
    fi
  fi
  if grep -q 'func_2_svc:' temp ; then
    if [ $switch_func_2_svc_found == 0 ]; then
      switch_func_2_svc_found=1
      if grep -q '[Yy][Ee][Ss]' temp ; then
        switch_func_2_svc=1
      fi
      rm temp
      continue
    fi
    if [ $switch_func_2_svc_found != 0 ]; then
      echo "Please check line "$iline
      echo "func_2_svc has been set in the fore part"
      rm temp
      rm temp_preset
      exit 1
    fi
  fi
  rm temp
done
if [ $switch_func_1_tuple_found == 0 ]; then
  echo "You should put switch_func_1_tuple between a pair of auto_gen_preset!!!"
  rm temp_preset
  exit 1
fi
if [ $switch_func_2_svc_found == 0 ]; then
  echo "You should put switch_func_2_svc between a pair of auto_gen_preset!!!"
  rm temp_preset
  exit 1
fi
echo "Name of Algorithm is "$algorithm_name
echo "Templates are: "
echo "  "$head_name
echo "  "$source_name
echo "Targets are: "
echo "  "$head_name_new
echo "  "$source_name_new
if [ $switch_func_1_tuple == 0 ]; then
  echo "func_1_tuple   will NOT   be called!"
fi
if [ $switch_func_1_tuple == 1 ]; then
  echo "func_1_tuple   will       be called!!!"
fi
if [ $switch_func_2_svc == 0 ]; then
  echo "func_2_svc     will NOT   be called!"
fi
if [ $switch_func_2_svc == 1 ]; then
  echo "func_2_svc     will       be called!!!"
fi
echo ""
if [ $switch_func_2_svc == 1 ]; then
  echo "This program will deal with svc according to "$manager_svc_name
  echo "Make sure this file is examined"
fi
echo "Please check information above carefully, cause this script will change your target files."
echo "If you want to change something, please quit and modify related files"
read -n1 -p "Press Y/y to confirm. Press any key else to quit." continue_or_not
if [ $continue_or_not != Y -a $continue_or_not != y ]; then
  echo ""
  echo "You chose to quit."
  rm temp_preset
  exit 1
fi
rm temp_preset
#**************************************read card func_1_tuple**********************************************
if [ $switch_func_1_tuple == 1 ]; then
  num_marker_tuple=`gawk 'BEGIN{RS="//auto_gen_tuple\n";i=0};{i++};END{print i}' $card_name`
  ((num_marker_tuple--))
  if [ $num_marker_tuple != 2 ]; then
    echo "Please check auto_gen_tuple in "$card_name
    echo "There should be exactly one pair of this mark, but we have found "$num_marker_tuple" marks!"
    exit 1
  fi
  gawk 'BEGIN{FS="//auto_gen_tuple\n";RS="You will ont find me, will you?";ORS=""};{print $2}' $card_name > temp_tuple
  num_tuple=0
  num_branch=0
  num_sizeVar=0
  declare -a array_tuple
  declare -a array_branch_belong
  declare -a array_branch_size
  declare -a array_branch_kind
  declare -a array_branch_type
  declare -a array_branch
  declare -a array_branch_is_sizeVar
  declare -a array_branch_min
  declare -a array_branch_max
  declare -a array_branch_var_as_array
  declare -a array_sizeVar
  declare -a array_sizeVar_min
  declare -a array_sizeVar_max
  num_line_temp_tuple=`gawk '{i++};END{print i}' temp_tuple`
  for (( iline=1; iline<=num_line_temp_tuple; iline++ ))
  do 
    sed -n "$iline s/\(.*\)/\1/p" temp_tuple > temp
    if grep -E -q '^ *$|^#|^ *//' temp ; then
      rm temp
      continue
    fi
    if grep -q '^Tuple' temp ; then
      array_tuple[((num_tuple++))]=`gawk '{print $2}' temp`
      echo ""
      echo NTuple ${array_tuple[((num_tuple-1))]}
      rm temp
      continue
    fi
    if [ $num_tuple == 0 ]; then
      echo "Please check line "$iline"!!!"
      echo "You should put a Tuplename before its branch!!!"
      rm temp
      rm temp_tuple
      exit 1
    fi
    branch_kind=`gawk '{print $1}' temp`
    branch_type=`gawk '{print $2}' temp`
    branch_name=`gawk '{print $3}' temp`
    if [ $branch_kind != "Array" -a $branch_kind != "Item" ]; then
      echo "Please check line "$iline"!!!"
      echo "Cannot deal with this kind ( "$branch_kind" ) of TBranch yet!!!"
      rm temp
      rm temp_tuple
      exit 1
    fi
    branch_declared=0
    for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
    do
      if [ ${array_branch_belong[iter_branch]} == ${array_tuple[((num_tuple-1))]} ]; then
        if [ ${array_branch[iter_branch]} == $branch_name ]; then
          branch_declared=1
          break
        fi
      fi
    done
    if [ $branch_declared == 1 ]; then
      echo "Please check line "$iline"!!!"
      echo "There is a TBranch named "$branch_name" in the fore part!!!"
      rm temp
      rm temp_tuple
      exit 1
    fi
    echo ""
    array_branch_is_sizeVar[num_branch]=0
    if grep -q 'size_variable' temp ; then
      if [ $branch_kind == "Array" ]; then
        echo "Please check line "$iline"!!!"
        echo "size_variable cannot be an Array!!!"
        rm temp
        rm temp_tuple
        exit 1
      fi
      array_sizeVar[num_sizeVar]=$branch_name
      array_sizeVar_min[num_sizeVar]=`gawk '{print $4}' temp`
      array_sizeVar_max[num_sizeVar]=`gawk '{print $5}' temp`
      ((num_sizeVar++))
      array_branch_is_sizeVar[num_branch]=1
      array_branch_min[num_branch]=`gawk '{print $4}' temp`
      array_branch_max[num_branch]=`gawk '{print $5}' temp`
      echo "     TBranch "$branch_name" is a size_variable! It will be responsible to following Arrays!"
      echo "          min: "${array_branch_min[num_branch]}
      echo "          max: "${array_branch_max[num_branch]}
    fi
    if [ $branch_kind == "Array" ]; then
      array_branch_var_as_array[num_branch]=0
      if grep -q 'var_as_array' temp ; then
        array_branch_var_as_array[num_branch]=1
      fi
      if ! grep -q 'size_fixed' temp ; then
        array_branch_size[num_branch]=${array_sizeVar[((num_sizeVar-1))]}
        array_branch_is_fixed[num_branch]=0
        array_branch_min[num_branch]=${array_sizeVar_min[((num_sizeVar-1))]}
        array_branch_max[num_branch]=${array_sizeVar_max[((num_sizeVar-1))]}
        echo "     TBranch "$branch_name" is an Array!"
        echo "     "${array_sizeVar[((num_sizeVar-1))]}" is its sizeVariable!"
        echo "          min: "${array_branch_min[num_branch]}
        echo "          max: "${array_branch_max[num_branch]}
        echo "          var as array? "${array_branch_var_as_array[num_branch]}
      fi
      if grep -q 'size_fixed' temp ; then
        array_branch_size[num_branch]=`gawk '{print $4}' temp`
        array_branch_is_fixed[num_branch]=1
        echo "     TBranch "$branch_name" is an Array!"
        echo "     Its size is fixed to "${array_branch_size[num_branch]}"!"
        echo "          var as array? "${array_branch_var_as_array[num_branch]}
      fi
    fi
    array_branch_belong[num_branch]=${array_tuple[((num_tuple-1))]}
    array_branch_kind[num_branch]=$branch_kind
    array_branch_type[num_branch]=$branch_type
    array_branch[num_branch]=$branch_name
    echo "     TBranch: "$branch_name
    echo "          belong to "${array_tuple[((num_tuple-1))]}
    echo "          kind: "$branch_kind
    echo "          type: "$branch_type
    ((num_branch++))
    rm temp
  done
  rm temp_tuple
fi
#******************************************read card func_2_svc**********************************************
if [ $switch_func_2_svc == 1 ]; then
  num_marker_svc=`gawk 'BEGIN{RS="//auto_gen_svc\n";i=0};{i++};END{print i}' $card_name`
  ((num_marker_svc--))
  if [ $num_marker_svc != 2 ]; then
    echo "Please check auto_gen_svc in "$card_name
    echo "There should be exactly one pair of this mark, but we have found "$num_marker_svc" marks!"
    exit 1
  fi
  gawk 'BEGIN{FS="//auto_gen_svc\n";RS="You will ont find me, will you?";ORS=""};{print $2}' $card_name > temp_svc
  num_svc=0
  declare -a array_svc
  declare -a array_svc_namespace
  declare -a array_svc_classI
  declare -a array_svc_classII
  declare -a array_svc_pService
  declare -a array_svc_path
  declare -a array_svc_pDir
  declare -a array_svc_StatusControl
  num_line_temp_svc=`gawk '{i++};END{print i}' temp_svc`
  for (( iline=1; iline<=num_line_temp_svc; iline++ ))
  do 
    sed -n "$iline s/\(.*\)/\1/p" temp_svc > temp
    if grep -E -q '^ *$|^#|^ *//' temp ; then
      rm temp
      continue
    fi
    array_svc[num_svc]=`gawk 'BEGIN{FS=" ";RS="\n"};{print $1}' temp`
    ((num_svc++))
  done
  rm temp_svc
  #look up to the dictionary
  num_marker_svcTable=`gawk 'BEGIN{RS="//auto_gen_svcTable\n";i=0};{i++};END{print i}' $manager_svc_name`
  ((num_marker_svcTable--))
  if [ $num_marker_svcTable != 2 ]; then
    echo "Please check auto_gen_svcTable in "$manager_svc_name
    echo "There should be exactly one pair of this mark, but we have found "$num_marker_svcTable" marks!"
    exit 1
  fi
  gawk 'BEGIN{FS="//auto_gen_svcTable\n";RS="You will ont find me, will you?";ORS=""};{print $2}' $manager_svc_name > temp_svcTable
  num_line_temp_svcTable=`gawk '{i++};END{print i}' temp_svcTable`
  for (( iline=1; iline<=num_line_temp_svcTable; iline++ ))
  do 
    sed -n "$iline s/\(.*\)/\1/p" temp_svcTable > temp
    if grep -E -q '^ *$|^#|^ *//' temp ; then
      rm temp
      continue
    fi
    if grep -q 'classI:' temp ; then
      tempVar_svc_classI=`gawk '{print $2}' temp`
    elif grep -q 'classII:' temp ; then
      #Abnormal examine: no classI before
      tempVar_svc_classII=`gawk '{print $2}' temp`
    else
      #Abnormal examine: no classI/classII before
      tempVar_svc=`gawk '{print $2}' temp`
      for (( iter_svc=0; iter_svc<num_svc; iter_svc++ ))
      do
        if [ ${array_svc[iter_svc]} == $tempVar_svc ]; then
          echo "Svc: "${array_svc[iter_svc]}
          array_svc_classI[iter_svc]=$tempVar_svc_classI
          echo "     classI: "${array_svc_classI[iter_svc]}
          if [ $tempVar_svc_classI = "other" ]; then
            keyword="//auto_gen_svcDescribe_"${array_svc[iter_svc]}"\n"
            num_marker_svcDescribe=`gawk -v "gawk_keyword=$keyword" 'BEGIN{RS=gawk_keyword;i=0};{i++};END{print i}' $manager_svc_name`
            ((num_marker_svcDescribe--))
            if [ $num_marker_svcDescribe != 2 ]; then
              echo "Please check "$keyword" in "$manager_svc_name
              echo "There should be exactly one pair of this mark, but we have found "$num_marker_svcDescribe" marks!"
              rm temp_svcTable
              exit 1
            fi
            gawk -v "gawk_keyword=$keyword" 'BEGIN{FS=gawk_keyword;RS="You will not find me, will you?";ORS=""};{print $2}' $manager_svc_name > temp_svcDescribe_${array_svc[iter_svc]}
            array_svc_StatusControl[iter_svc]=`sed -n 's/\(.*\)StatusControl:\(.*\)/\2/p' temp`
            echo "     StatusControl: "${array_svc_StatusControl[iter_svc]}
          fi
          if [ $tempVar_svc_classI = "SmartDataPtr" ]; then
            array_svc_classII[iter_svc]=$tempVar_svc_classII
            array_svc_namespace[iter_svc]=`gawk '{print $1}' temp`
            array_svc_pService[iter_svc]=`gawk '{print $3}' temp`
            array_svc_StatusControl[iter_svc]="!ptr_$tempVar_svc"
            echo "     classII: "${array_svc_classII[iter_svc]}
            echo "     namespace: "${array_svc_namespace[iter_svc]}
            echo "     pService: "${array_svc_pService[iter_svc]}
            echo "     StatusControl: "${array_svc_StatusControl[iter_svc]}
            if [ $tempVar_svc_classII == "pSer_path" ]; then
              array_svc_path[iter_svc]=`gawk '{print $4}' temp`
              echo "     path: "${array_svc_path[iter_svc]}
            fi
            if [ $tempVar_svc_classII == "pSer_pDir" ]; then
              array_svc_pDir[iter_svc]=`gawk '{print $4}' temp`
              echo "     pDir: "${array_svc_pDir[iter_svc]}
            fi
          fi
        fi
      done
    fi
    rm temp
  done
  rm temp_svcTable
fi
#**********************************************modify .cxx**********************************************
cat $source_name > $source_name_new
num_line_source=`gawk '{i++};END{print i}' $source_name_new`
for (( iline=1; iline<=num_line_source; iline++ ))
do 
  sed -n "$iline s/\(.*\)/\1/p" $source_name_new > temp
  if grep -q '^ *//auto_gen' temp ; then
    indent_forIline=`sed -n "s/\(.*\)\/\/auto_gen\(.*\)/\1/p" temp`
    auto_gen_part_1=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $3}' temp`
    auto_gen_part_2=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $4}' temp`
    auto_gen_part_3=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $5}' temp`
    touch temp_forIline_insert
#if loop
    if [ $auto_gen_part_1 == "loop" ]; then
      if [ $auto_gen_part_2 == "charged" ]; then
        echo "$indent_forIline""for ( int i_"$auto_gen_part_3" = 0; i_"$auto_gen_part_3" < ptr_EvtRecEvent -> totalCharged(); i_"$auto_gen_part_3"++ ){" >> temp_forIline_insert
        echo "$indent_forIline""  EvtRecTrackIterator itr_EvtRecTrack_"$auto_gen_part_3" = ptr_EvtRecTrackCol -> begin() + i_"$auto_gen_part_3";" >> temp_forIline_insert
      fi
    fi
#if tuple
    if [ $switch_func_1_tuple == 1 ]; then
      if [ $auto_gen_part_1 == "tuple" ]; then
        if [ $auto_gen_part_2 == "initialize" ]; then
          echo "$indent_forIline""StatusCode status;" >> temp_forIline_insert
          for (( iter_tuple=0; iter_tuple<num_tuple; iter_tuple++ ))
          do
            echo "" >> temp_forIline_insert
            echo "$indent_forIline""NTuplePtr NTPtr_"${array_tuple[iter_tuple]}"( ntupleSvc(), \"FILE1/"${array_tuple[iter_tuple]}"\" );" >>temp_forIline_insert
            echo "$indent_forIline""if ( NTPtr_"${array_tuple[iter_tuple]}" ) m_tuple_"${array_tuple[iter_tuple]}" = NTPtr_"${array_tuple[iter_tuple]}";" >> temp_forIline_insert
            echo "$indent_forIline""else {" >> temp_forIline_insert
            echo "$indent_forIline""  m_tuple_"${array_tuple[iter_tuple]}" = ntupleSvc()->book (\"FILE1/"${array_tuple[iter_tuple]}"\", CLID_ColumnWiseTuple, \"ks N-Tuple example\");" >> temp_forIline_insert
            echo "$indent_forIline""  if ( m_tuple_"${array_tuple[iter_tuple]}" ){" >> temp_forIline_insert
            echo "" >> temp_forIline_insert
            for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
            do
              if [ ${array_branch_belong[iter_branch]} != ${array_tuple[iter_tuple]} ]; then
                continue
              fi
              if [ ${array_branch_kind[iter_branch]} == "Item" -a ${array_branch_is_sizeVar[iter_branch]} == 0 ]; then
                echo "$indent_forIline""    status = m_tuple_"${array_tuple[iter_tuple]}"->addItem ( \""${array_branch[iter_branch]}"\", m_"${array_tuple[iter_tuple]}"_"${array_branch[iter_branch]}" );" >> temp_forIline_insert
              fi
              if [ ${array_branch_kind[iter_branch]} == "Item" -a ${array_branch_is_sizeVar[iter_branch]} == 1 ]; then
                echo "$indent_forIline""    status = m_tuple_"${array_tuple[iter_tuple]}"->addItem ( \""${array_branch[iter_branch]}"\", m_"${array_tuple[iter_tuple]}"_"${array_branch[iter_branch]}", "${array_branch_min[iter_branch]}", "${array_branch_max[iter_branch]}" );" >> temp_forIline_insert
              fi
              if [ ${array_branch_kind[iter_branch]} == "Array" ]; then
                if [ ${array_branch_is_fixed[iter_branch]} == 0 ]; then
                  echo "$indent_forIline""    status = m_tuple_"${array_tuple[iter_tuple]}"->addItem ( \""${array_branch[iter_branch]}"\", m_"${array_branch_size[iter_branch]}", m_"${array_tuple[iter_tuple]}"_"${array_branch[iter_branch]}" );" >> temp_forIline_insert
                fi
                if [ ${array_branch_is_fixed[iter_branch]} == 1 ]; then
                  echo "$indent_forIline""    status = m_tuple_"${array_tuple[iter_tuple]}"->addItem ( \""${array_branch[iter_branch]}"\", "${array_branch_size[iter_branch]}", m_"${array_tuple[iter_tuple]}"_"${array_branch[iter_branch]}" );" >> temp_forIline_insert
                fi
              fi
            done
            echo "" >> temp_forIline_insert
            echo "$indent_forIline""  }" >> temp_forIline_insert
            echo "$indent_forIline""  else    {" >> temp_forIline_insert
            echo "$indent_forIline""    log << MSG::ERROR << \"    Cannot book N-tuple:\" << long( m_tuple_"${array_tuple[iter_tuple]}" ) << endmsg;" >> temp_forIline_insert
            echo "$indent_forIline""    return StatusCode::FAILURE;" >> temp_forIline_insert
            echo "$indent_forIline""  }" >> temp_forIline_insert
            echo "$indent_forIline""}" >> temp_forIline_insert
          done
        fi
        if [ $auto_gen_part_2 == "variableDeclare" ]; then
          for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
          do
            if [ ${array_branch_kind[iter_branch]} == "Item" ];then
              echo "$indent_forIline"""${array_branch_type[iter_branch]}" "${array_branch_belong[iter_branch]}"_"${array_branch[iter_branch]}" = 0;" >> temp_forIline_insert
            fi
            if [ ${array_branch_kind[iter_branch]} == "Array" ];then
              if [ ${array_branch_var_as_array[iter_branch]} == 1 ];then
                echo "$indent_forIline"""${array_branch_type[iter_branch]}" "${array_branch_belong[iter_branch]}"_"${array_branch[iter_branch]}"["${array_branch_size[iter_branch]}"];" >> temp_forIline_insert
              fi
              if [ ${array_branch_var_as_array[iter_branch]} == 0 ];then
                echo "$indent_forIline""vector<"${array_branch_type[iter_branch]}"> "${array_branch_belong[iter_branch]}"_"${array_branch[iter_branch]}";" >> temp_forIline_insert
              fi
            fi
          done
        fi
        if [ $auto_gen_part_2 == "write" ]; then
          # you'd better check auto_gen_part_3
          for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
          do
            if [ ${array_branch_belong[iter_branch]} != $auto_gen_part_3 ]; then
              continue
            fi
            if [ ${array_branch_kind[iter_branch]} == "Item" ];then
              echo "$indent_forIline""m_"$auto_gen_part_3"_"${array_branch[iter_branch]}" = "$auto_gen_part_3"_"${array_branch[iter_branch]}";" >> temp_forIline_insert
            fi
            if [ ${array_branch_kind[iter_branch]} == "Array" ];then
              if [ ${array_branch_is_fixed[iter_branch]} == 1 ];then
                echo "$indent_forIline""for ( int i = 0; i < "${array_branch_size[iter_branch]}"; i++ ){" >> temp_forIline_insert
                echo "$indent_forIline""  m_"$auto_gen_part_3"_"${array_branch[iter_branch]}"[i] = "$auto_gen_part_3"_"${array_branch[iter_branch]}"[i];" >> temp_forIline_insert
                echo "$indent_forIline""}" >> temp_forIline_insert
              fi
              if [ ${array_branch_is_fixed[iter_branch]} == 0 ];then
                echo "$indent_forIline""for ( int i = 0; i < "${array_branch_size[iter_branch]}"; i++ ){" >> temp_forIline_insert
                echo "$indent_forIline""  if ( i >= "${array_branch_max[iter_branch]}" ){" >> temp_forIline_insert
                echo "$indent_forIline""    break;" >> temp_forIline_insert
                echo "$indent_forIline""  }" >> temp_forIline_insert
                echo "$indent_forIline""  m_"$auto_gen_part_3"_"${array_branch[iter_branch]}"[i] = "$auto_gen_part_3"_"${array_branch[iter_branch]}"[i];" >> temp_forIline_insert
                echo "$indent_forIline""}" >> temp_forIline_insert
              fi
            fi
          done
          echo "" >> temp_forIline_insert
          echo "$indent_forIline""m_tuple_"$auto_gen_part_3" -> write();" >> temp_forIline_insert
        fi
        if [  $auto_gen_part_2 == "getValue" ]; then
          # you'd better check auto_gen_part_3
          for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
          do
            if [ ${array_branch_belong[iter_branch]} == $auto_gen_part_3 ]; then
              echo "$indent_forIline"$auto_gen_part_3"_"${array_branch[iter_branch]}" = mdcTrk -> "${array_branch[iter_branch]}"();" >> temp_forIline_insert
            fi
          done
        fi
      fi
    fi
#if svc
    if [ $switch_func_2_svc == 1 ]; then
      if [ $auto_gen_part_1 == "svc" ]; then
        if [ $auto_gen_part_2 == "Ncut" ]; then
          for (( iter_svc=0; iter_svc<num_svc; iter_svc++ ))
          do
            echo "$indent_forIline""int Ncut_"${array_svc[iter_svc]}" = 0;" >> temp_forIline_insert
          done
        fi
        if [ $auto_gen_part_2 == "declare" ]; then
          for (( iter_svc=0; iter_svc<num_svc; iter_svc++ ))
          do
            if [ ${array_svc_classI[iter_svc]} == "SmartDataPtr" ]; then
              if [ ${array_svc_classII[iter_svc]} == "pSer_path" ]; then
                if [ ${array_svc_namespace[iter_svc]} == "Null" ]; then
                  echo "$indent_forIline""SmartDataPtr<"${array_svc[iter_svc]}"> ptr_"${array_svc[iter_svc]}"( "${array_svc_pService[iter_svc]}"(), \"/"${array_svc_path[iter_svc]}"/"${array_svc[iter_svc]}"\" );" >> temp_forIline_insert
                else
                  echo "$indent_forIline""SmartDataPtr<"${array_svc_namespace[iter_svc]}"::"${array_svc[iter_svc]}"> ptr_"${array_svc[iter_svc]}"( "${array_svc_pService[iter_svc]}"(), \"/"${array_svc_path[iter_svc]}"/"${array_svc[iter_svc]}"\" );" >> temp_forIline_insert
                fi
              fi
              if [ ${array_svc_classII[iter_svc]} == "pSer_pDir" ]; then
                if [ ${array_svc_namespace[iter_svc]} == "Null" ]; then
                  echo "$indent_forIline""SmartDataPtr<"${array_svc[iter_svc]}"> ptr_"${array_svc[iter_svc]}"( "${array_svc_pService[iter_svc]}"(), "${array_svc_pDir[iter_svc]}"::"${array_svc[iter_svc]}" );" >> temp_forIline_insert
                else
                  echo "$indent_forIline""SmartDataPtr<"${array_svc_namespace[iter_svc]}"::"${array_svc[iter_svc]}"> ptr_"${array_svc[iter_svc]}"( "${array_svc_pService[iter_svc]}"(), "${array_svc_pDir[iter_svc]}"::"${array_svc[iter_svc]}" );" >> temp_forIline_insert
                fi
              fi
            fi
            if [ ${array_svc_classI[iter_svc]} == "other" ]; then
              sed -i "s/\(.*\)/$indent_forIline\1/g" temp_svcDescribe_${array_svc[iter_svc]}
              cat temp_svcDescribe_${array_svc[iter_svc]} >> temp_forIline_insert
              rm temp_svcDescribe_${array_svc[iter_svc]}
            fi
            echo "$indent_forIline""if( "${array_svc_StatusControl[iter_svc]}" ){" >> temp_forIline_insert
            echo "$indent_forIline""  log << MSG::ERROR << \"Can not get "${array_svc[iter_svc]}"\" << endreq;" >> temp_forIline_insert
            echo "$indent_forIline""  return StatusCode::SUCCESS;" >> temp_forIline_insert
            echo "$indent_forIline""}" >> temp_forIline_insert
            echo "$indent_forIline""Ncut_"${array_svc[iter_svc]}"++;" >> temp_forIline_insert
            #deal with other class
          done
        fi
        if [ $auto_gen_part_2 == "cout" ]; then
          for (( iter_svc=0; iter_svc<num_svc; iter_svc++ ))
          do
            echo "$indent_forIline""std::cout<<\""${array_svc[iter_svc]}" successed = \"<<Ncut_"${array_svc[iter_svc]}"<<std::endl;" >> temp_forIline_insert
          done
        fi
      fi
    fi
#modify target file
    gawk -v gawk_iline=$iline 'BEGIN{RS="\n";ORS="\n";i=1};{if(i<gawk_iline)print;i++}' $source_name_new > temp_forIline_1
    gawk -v gawk_iline=$iline 'BEGIN{RS="\n";ORS="\n";i=1};{if(i>gawk_iline)print;i++}' $source_name_new > temp_forIline_2
    gawk '//' temp_forIline_1 temp_forIline_insert temp_forIline_2 > $source_name_new
    num_line_temp_forIline_insert=`gawk '{i++};END{print i}' temp_forIline_insert`
    (( num_line_source=num_line_source+num_line_temp_forIline_insert-1 ))
    (( iline=iline+num_line_temp_forIline_insert-1 ))
    rm temp_forIline_insert
    rm temp_forIline_1
    rm temp_forIline_2
  fi
  rm temp
done
#**********************************************modify .h**********************************************
cat $head_name > $head_name_new
num_line_head=`gawk '{i++};END{print i}' $head_name_new`
for (( iline=1; iline<=num_line_head; iline++ ))
do 
  sed -n "$iline s/\(.*\)/\1/p" $head_name_new > temp
  if grep -q '^ *//auto_gen' temp ; then
    indent_forIline=`sed -n "s/\(.*\)\/\/auto_gen\(.*\)/\1/p" temp`
    auto_gen_part_1=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $3}' temp`
    auto_gen_part_2=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $4}' temp`
    auto_gen_part_3=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $5}' temp`
    auto_gen_part_4=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $6}' temp`
    touch temp_forIline_insert
#if tuple
    if [ $switch_func_1_tuple == 1 ]; then
      if [ $auto_gen_part_1 == "tuple" ]; then
        if [ $auto_gen_part_2 == "declare" ]; then
          for (( iter_tuple=0; iter_tuple<num_tuple; iter_tuple++ ))
          do
            echo "" >> temp_forIline_insert
            echo "$indent_forIline""NTuple::Tuple*  m_tuple_"${array_tuple[iter_tuple]}";" >> temp_forIline_insert
            blank_line_printed=0
            for ((iter_branch=0; iter_branch<num_branch; iter_branch++ ))
            do
              if [ ${array_branch_belong[iter_branch]} != ${array_tuple[iter_tuple]} ]; then
                continue
              fi
              if [ $blank_line_printed == 0 ]; then
                echo "" >> temp_forIline_insert
                blank_line_printed=1
              fi
              echo "$indent_forIline""NTuple::"${array_branch_kind[iter_branch]}"<"${array_branch_type[iter_branch]}"> m_"${array_tuple[iter_tuple]}"_"${array_branch[iter_branch]}";" >> temp_forIline_insert
            done
          done
        fi
      fi
    fi
#modify target file
    gawk -v gawk_iline=$iline 'BEGIN{RS="\n";ORS="\n";i=1};{if(i<gawk_iline)print;i++}' $head_name_new > temp_forIline_1
    gawk -v gawk_iline=$iline 'BEGIN{RS="\n";ORS="\n";i=1};{if(i>gawk_iline)print;i++}' $head_name_new > temp_forIline_2
    gawk '//' temp_forIline_1 temp_forIline_insert temp_forIline_2 > $head_name_new
    num_line_temp_forIline_insert=`gawk '{i++};END{print i}' temp_forIline_insert`
    (( num_line_head=num_line_head+num_line_temp_forIline_insert-1 ))
    (( iline=iline+num_line_temp_forIline_insert-1 ))
    rm temp_forIline_insert
    rm temp_forIline_1
    rm temp_forIline_2
  fi
  rm temp
done
