#!/bin/bash
#**********************************************preset****************************************************
algorithm_name=$1
algorithm_path=$2
echo algorithm_name=$algorithm_name
echo algorithm_path=$algorithm_path
auto_gen_dir=$MYHOME"/share/auto_gen"
card_name=$algorithm_path"/auto_gen/auto_gen_card.txt"
head_name=$algorithm_path"/auto_gen/"$algorithm_name".h"
head_name_new=$algorithm_path"/"$algorithm_name"/"$algorithm_name".h"
source_name=$algorithm_path"/auto_gen/"$algorithm_name".cxx"
source_name_new=$algorithm_path"/src/"$algorithm_name".cxx"
jobOption_name=$algorithm_path"/auto_gen/jobOptions_"$algorithm_name".txt"
jobOption_name_new=$algorithm_path"/share/jobOptions_"$algorithm_name".txt"
manager_svc_name=$auto_gen_dir"/auto_gen_manager_svc.txt"
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
switch_func_3_property=0
switch_func_1_tuple_found=0
switch_func_2_svc_found=0
switch_func_3_property_found=0
num_line=`gawk '{i++};END{print i}' temp_preset`
for (( iline=1; iline<=num_line; iline++ ))
do 
  sed -n "$iline s/\(.*\)/\1/p" temp_preset> temp
  if grep -P -q '^[ \t]*$|^#|^[ \t]*//' temp ; then
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
  if grep -q 'func_3_property:' temp ; then
    if [ $switch_func_3_property_found == 0 ]; then
      switch_func_3_property_found=1
      if grep -q '[Yy][Ee][Ss]' temp ; then
        switch_func_3_property=1
      fi
      rm temp
      continue
    fi
    if [ $switch_func_3_property_found != 0 ]; then
      echo "Please check line "$iline
      echo "func_3_property has been set in the fore part"
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
if [ $switch_func_3_property_found == 0 ]; then
  echo "You should put switch_func_3_property between a pair of auto_gen_preset!!!"
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
if [ $switch_func_3_property == 0 ]; then
  echo "func_3_property     will NOT   be called!"
fi
if [ $switch_func_3_property == 1 ]; then
  echo "func_3_property     will       be called!!!"
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
  declare -a array_branch_step
  declare -a array_branch_value
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
    if grep -P -q '^[ \t]*$|^#|^[ \t]*//' temp ; then
      rm temp
      continue
    fi
    if grep -q '^Tuple' temp ; then
      array_tuple[((num_tuple++))]=`gawk '{print $2}' temp`
#      echo ""
#      echo NTuple ${array_tuple[((num_tuple-1))]}
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
#    echo ""
    array_branch_is_sizeVar[num_branch]=0
    if grep -q 'size_variable' temp ; then
      if [[ $branch_kind == "Array" ]]; then
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
      #echo "     TBranch "$branch_name" is a size_variable! It will be responsible to following Arrays!"
      #echo "          min: "${array_branch_min[num_branch]}
      #echo "          max: "${array_branch_max[num_branch]}
    fi
    if [[ $branch_kind == "Array" ]]; then
      array_branch_var_as_array[num_branch]=0
      if grep -q 'var_as_array' temp ; then
        array_branch_var_as_array[num_branch]=1
      fi
      if ! grep -q 'size_fixed' temp ; then
        array_branch_size[num_branch]=${array_sizeVar[((num_sizeVar-1))]}
        array_branch_is_fixed[num_branch]=0
        array_branch_min[num_branch]=${array_sizeVar_min[((num_sizeVar-1))]}
        array_branch_max[num_branch]=${array_sizeVar_max[((num_sizeVar-1))]}
        #echo "     TBranch "$branch_name" is an Array!"
        #echo "     "${array_sizeVar[((num_sizeVar-1))]}" is its sizeVariable!"
        #echo "          min: "${array_branch_min[num_branch]}
        #echo "          max: "${array_branch_max[num_branch]}
        #echo "          var as array? "${array_branch_var_as_array[num_branch]}
      fi
      if grep -q 'size_fixed' temp ; then
        array_branch_size[num_branch]=`gawk '{print $4}' temp`
        array_branch_is_fixed[num_branch]=1
        #echo "     TBranch "$branch_name" is an Array!"
        #echo "     Its size is fixed to "${array_branch_size[num_branch]}"!"
        #echo "          var as array? "${array_branch_var_as_array[num_branch]}
      fi
    fi
    cat temp > temp_forSTEP
    sed -i "s/.*STEP: *\(\w*\)* *.*/\1/g" temp_forSTEP
    array_branch_step[iter_branch]=`gawk '{print $1}' temp_forSTEP`
    rm temp_forSTEP
    cat temp > temp_forVALUE
    sed -i "s/.*VALUE:\(.*\)/\1/g" temp_forVALUE
    array_branch_value[iter_branch]="`cat temp_forVALUE`"
    rm temp_forVALUE
    array_branch_belong[num_branch]=${array_tuple[((num_tuple-1))]}
    array_branch_kind[num_branch]=$branch_kind
    array_branch_type[num_branch]=$branch_type
    array_branch[num_branch]=$branch_name
    #echo "     TBranch: "$branch_name
    #echo "          belong to "${array_tuple[((num_tuple-1))]}
    #echo "          kind: "$branch_kind
    #echo "          type: "$branch_type
    #echo "          step: "${array_branch_step[num_branch]}
    #echo "          value: ""${array_branch_value[num_branch]}"
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
    if grep -P -q '^[ \t]*$|^#|^[ \t]*//' temp ; then
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
    if grep -P -q '^[ \t]*$|^#|^[ \t]*//' temp ; then
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
#      echo "*****tempVar_svc = "$tempVar_svc
      for (( iter_svc=0; iter_svc<num_svc; iter_svc++ ))
      do
#       echo "     array_svc="${array_svc[iter_svc]}
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
            #echo "     classII: "${array_svc_classII[iter_svc]}
            #echo "     namespace: "${array_svc_namespace[iter_svc]}
            #echo "     pService: "${array_svc_pService[iter_svc]}
            #echo "     StatusControl: "${array_svc_StatusControl[iter_svc]}
            if [[ $tempVar_svc_classII == "pSer_path" ]]; then
              array_svc_path[iter_svc]=`gawk '{print $4}' temp`
              #echo "     path: "${array_svc_path[iter_svc]}
            fi
            if [[ $tempVar_svc_classII == "pSer_pDir" ]]; then
              array_svc_pDir[iter_svc]=`gawk '{print $4}' temp`
              #echo "     pDir: "${array_svc_pDir[iter_svc]}
            fi
          fi
        fi
      done
    fi
    rm temp
  done
  rm temp_svcTable
fi
#******************************************read card func_3_property**********************************************
if [ $switch_func_3_property == 1 ]; then
  num_marker_property=`gawk 'BEGIN{RS="//auto_gen_property\n";i=0};{i++};END{print i}' $card_name`
  ((num_marker_property--))
  if [ $num_marker_property != 2 ]; then
    echo "Please check auto_gen_property in "$card_name
    echo "There should be exactly one pair of this mark, but we have found "$num_marker_property" marks!"
    exit 1
  fi
  gawk 'BEGIN{FS="//auto_gen_property\n";RS="You will ont find me, will you?";ORS=""};{print $2}' $card_name > temp_property
  num_property=0
  declare -a array_property
  declare -a array_property_type
  declare -a array_property_value
  num_line_temp_property=`gawk '{i++};END{print i}' temp_property`
  for (( iline=1; iline<=num_line_temp_property; iline++ ))
  do 
    sed -n "$iline s/\(.*\)/\1/p" temp_property > temp
    if grep -P -q '^[ \t]*$|^#|^[ \t]*//' temp ; then
      rm temp
      continue
    fi
    array_property_type[num_property]=`gawk 'BEGIN{FS=" ";RS="\n"};{print $1}' temp`
    array_property[num_property]=`gawk 'BEGIN{FS=" ";RS="\n"};{print $2}' temp`
    array_property_value[num_property]=`gawk 'BEGIN{FS=" ";RS="\n"};{print $3}' temp`
    ((num_property++))
  done
  rm temp_property
fi
#**********************************************modify .cxx**********************************************
cat $source_name > $source_name_new
for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
do
  sed -i "s/\<${array_branch[iter_branch]}\>/${array_branch_belong[iter_branch]}_${array_branch[iter_branch]}/g" $source_name_new
done
num_line_source=`gawk '{i++};END{print i}' $source_name_new`
for (( iline=1; iline<=num_line_source; iline++ ))
do 
  sed -n "$iline s/\(.*\)/\1/p" $source_name_new > temp
  if grep -P -q '^[ \t]*//auto_gen' temp ; then
    indent_forIline=`sed -n "s/\(.*\)\/\/auto_gen\(.*\)/\1/p" temp`
    auto_gen_part_1=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $3}' temp`
    auto_gen_part_2=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $4}' temp`
    auto_gen_part_3=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $5}' temp`
    auto_gen_part_4=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $6}' temp`
    touch temp_forIline_insert
#if loop
    if [[ $auto_gen_part_1 == "loop" ]]; then
      if [[ $auto_gen_part_2 == "charged" ]]; then
        if [ -z "$auto_gen_part_3" ];then
          echo "$indent_forIline""for ( int i = 0; i < ptr_EvtRecEvent -> totalCharged(); i++ ){" >> temp_forIline_insert
          echo "$indent_forIline""  EvtRecTrackIterator iter_EvtRecTrack = ptr_EvtRecTrackCol -> begin() + i;" >> temp_forIline_insert
        else
          echo "$indent_forIline""for ( int i_"$auto_gen_part_3" = 0; i_"$auto_gen_part_3" < ptr_EvtRecEvent -> totalCharged(); i_"$auto_gen_part_3"++ ){" >> temp_forIline_insert
          echo "$indent_forIline""  EvtRecTrackIterator iter_EvtRecTrack_"$auto_gen_part_3" = ptr_EvtRecTrackCol -> begin() + i_"$auto_gen_part_3";" >> temp_forIline_insert
        fi
      fi
      if [[ $auto_gen_part_2 == "neutral" ]]; then
        if [ -z "$auto_gen_part_3" ];then
          echo "$indent_forIline""for ( int i = ptr_EvtRecEvent -> totalCharged(); i < ptr_EvtRecEvent -> totalTracks(); i++ ){" >> temp_forIline_insert
          echo "$indent_forIline""  EvtRecTrackIterator iter_EvtRecTrack = ptr_EvtRecTrackCol -> begin() + i;" >> temp_forIline_insert
        else
          echo "$indent_forIline""for ( int i_"$auto_gen_part_3" = ptr_EvtRecEvent -> totalCharged(); i_"$auto_gen_part_3" < ptr_EvtRecEvent -> totalTracks(); i_"$auto_gen_part_3"++ ){" >> temp_forIline_insert
          echo "$indent_forIline""  EvtRecTrackIterator iter_EvtRecTrack_"$auto_gen_part_3" = ptr_EvtRecTrackCol -> begin() + i_"$auto_gen_part_3";" >> temp_forIline_insert
        fi
      fi
    fi
#if get
    if [[ $auto_gen_part_1 == "get" ]]; then
      if [[ $auto_gen_part_2 == "RecMdcTrack" ]]; then
        echo "$indent_forIline""RecMdcTrack* ptr_RecMdcTrack;" >> temp_forIline_insert
        echo "$indent_forIline""if ( (*iter_EvtRecTrack) -> isMdcTrackValid() ){" >> temp_forIline_insert
        echo "$indent_forIline""  ptr_RecMdcTrack = (*iter_EvtRecTrack) -> mdcTrack();" >> temp_forIline_insert
        echo "$indent_forIline""}" >> temp_forIline_insert
        if [ -n "$auto_gen_part_3" ]; then
          if [[ $auto_gen_part_3 == "withCut" ]]; then
            echo "$indent_forIline""else{" >> temp_forIline_insert
            echo "$indent_forIline""  continue;" >> temp_forIline_insert
            echo "$indent_forIline""}" >> temp_forIline_insert
          fi
        fi
      fi
      if [[ $auto_gen_part_2 == "RecEmcShower" ]]; then
        echo "$indent_forIline""RecEmcShower* ptr_RecEmcShower;" >> temp_forIline_insert
        echo "$indent_forIline""if ( (*iter_EvtRecTrack) -> isEmcShowerValid() ){" >> temp_forIline_insert
        echo "$indent_forIline""  ptr_RecEmcShower = (*iter_EvtRecTrack) -> emcShower();" >> temp_forIline_insert
        echo "$indent_forIline""}" >> temp_forIline_insert
        if [ -n "$auto_gen_part_3" ]; then
          if [[ $auto_gen_part_3 == "withCut" ]]; then
            echo "$indent_forIline""else{" >> temp_forIline_insert
            echo "$indent_forIline""  continue;" >> temp_forIline_insert
            echo "$indent_forIline""}" >> temp_forIline_insert
          fi
        fi
      fi
      if [[ $auto_gen_part_2 == "PrimaryVertex" ]]; then
        echo "$indent_forIline""Hep3Vector xorigin( 0., 0., 0. );" >> temp_forIline_insert
        echo "$indent_forIline""IVertexDbSvc* svc_IVertexDbSvc;" >> temp_forIline_insert
        echo "$indent_forIline""Gaudi::svcLocator() -> service( \"VertexDbSvc\", svc_IVertexDbSvc );" >> temp_forIline_insert
        echo "$indent_forIline""if( svc_IVertexDbSvc -> isVertexValid() ){" >> temp_forIline_insert
        echo "$indent_forIline""  double* a_PV = svc_IVertexDbSvc -> PrimaryVertex();" >> temp_forIline_insert
        echo "$indent_forIline""  xorigin.setX( a_PV[0] );" >> temp_forIline_insert
        echo "$indent_forIline""  xorigin.setY( a_PV[1] );" >> temp_forIline_insert
        echo "$indent_forIline""  xorigin.setZ( a_PV[2] );" >> temp_forIline_insert
        echo "$indent_forIline""}" >> temp_forIline_insert
        echo "$indent_forIline""HepVector3D h3D_PrimaryVertex( xorigin[0], xorigin[1], xorigin[2] );" >> temp_forIline_insert
      fi
    fi
#if cut
    if [[ $auto_gen_part_1 == "cut" ]]; then
      cat auto_gen_cut_"$auto_gen_part_2".txt > temp_forCut
      sed -i "s/^\(.\)\(.*\)$/$indent_forIline\1\2/g" temp_forCut
      cat temp_forCut >> temp_forIline_insert
      rm temp_forCut
    fi
#if fit
    if [[ $auto_gen_part_1 == "fit" ]]; then
      if [[ $auto_gen_part_2 == "vertex" ]]; then
        num_var=$auto_gen_part_3
        declare -a array_par_verFit
        for (( i_var=0; i_var<num_var; i_var++ ))
        do
          ((i_var_p=i_var+6))
          array_par_verFit[i_var]=`gawk -v i=$i_var_p 'BEGIN{FS="_";RS="\n";ORS=""};{print $i}' temp`
        done
        for (( i_var=0; i_var<num_var; i_var++ ))
        do
          echo "$indent_forIline""WTrackParameter wTrk"$i_var";" >> temp_forIline_insert
        done
        for (( i_var=0; i_var<num_var; i_var++ ))
        do
          if [[ ${array_par_verFit[i_var]} == "k" ]]; then
            echo "$indent_forIline""wTrk"$i_var" = WTrackParameter( mk, trk_forVerFit_"$i_var" -> getZHelixK(), trk_forVerFit_"$i_var" -> getZErrorK() );" >> temp_forIline_insert
          elif [[ ${array_par_verFit[i_var]} == "pi" ]]; then
            echo "$indent_forIline""wTrk"$i_var" = WTrackParameter( mpi, trk_forVerFit_"$i_var" -> getZHelix(), trk_forVerFit_"$i_var" -> getZError() );" >> temp_forIline_insert
          elif [[ ${array_par_verFit[i_var]} == "p" ]]; then
            echo "$indent_forIline""wTrk"$i_var" = WTrackParameter( mp, trk_forVerFit_"$i_var" -> getZHelixP(), trk_forVerFit_"$i_var" -> getZErrorP() );" >> temp_forIline_insert
          elif [[ ${array_par_verFit[i_var]} == "e" ]]; then
            echo "$indent_forIline""wTrk"$i_var" = WTrackParameter( me, trk_forVerFit_"$i_var" -> getZHelixE(), trk_forVerFit_"$i_var" -> getZErrorE() );" >> temp_forIline_insert
          elif [[ ${array_par_verFit[i_var]} == "mu" ]]; then
            echo "$indent_forIline""wTrk"$i_var" = WTrackParameter( mmu, trk_forVerFit_"$i_var" -> getZHelixMu(), trk_forVerFit_"$i_var" -> getZErrorMu() );" >> temp_forIline_insert
          else
            echo "vertex fit cannot deal with partile "${array_par_verFit[$i_var]}"!!!"
            rm temp_forIline_insert
            rm temp
            exit 1
          fi
        done
        cat auto_gen_fit_"$auto_gen_part_2".txt > temp_forFit
        sed -i "s/^\(.\)\(.*\)$/$indent_forIline\1\2/g" temp_forFit
        cat temp_forFit >> temp_forIline_insert
        rm temp_forFit
        for (( i_var=0; i_var<num_var; i_var++ ))
        do
          echo "$indent_forIline""vtxfit->AddTrack( "$i_var", wTrk"$i_var" );" >> temp_forIline_insert
        done
        echo -n "$indent_forIline""vtxfit->AddVertex( 0, vxpar, " >> temp_forIline_insert
        for (( i_var=0; i_var<num_var; i_var++ ))
        do
          ((i_last=num_var-1))
          if [ $i_var == $i_last ]; then
            echo $i_var" );" >> temp_forIline_insert
          else
            echo -n $i_var", " >> temp_forIline_insert
          fi
        done
        echo "$indent_forIline""if( !vtxfit->Fit(0) ){" >> temp_forIline_insert
        echo "$indent_forIline""  return StatusCode::SUCCESS;" >> temp_forIline_insert
        echo "$indent_forIline""}" >> temp_forIline_insert
        echo "$indent_forIline""vtxfit->Swim(0);" >> temp_forIline_insert
        for (( i_var=0; i_var<num_var; i_var++ ))
        do
          echo "$indent_forIline""WTrackParameter wTrk_after_"$i_var" = vtxfit->wtrk( "$i_var" );" >> temp_forIline_insert
        done
      elif [[ $auto_gen_part_2 == "4c" ]]; then
        echo "$indent_forIline""KalmanKinematicFit * fit_4c_"$auto_gen_part_3" = KalmanKinematicFit::instance();" >> temp_forIline_insert
        echo "$indent_forIline""fit_4c_"$auto_gen_part_3"->init();" >> temp_forIline_insert
        declare -a array_par_4cFit_name
        declare -a array_par_4cFit_mass
#       safety check needed
        keyword="//auto_gen_fit_4c_"$auto_gen_part_3"\n"
        gawk -v gawk_keyword=$keyword 'BEGIN{FS=gawk_keyword;RS="You will ont find me, will you?";ORS=""};{print $2}' $card_name > temp_for4cFit
        num_var=`gawk '{i++};END{print i}' temp_for4cFit`
        for (( i_var=0; i_var<num_var; i_var++ ))
        do
          ((iline_for4cFit=i_var+1))
          sed -n "$iline_for4cFit s/\(.*\)/\1/p" temp_for4cFit > temp_for4cFit_ivar
          array_par_4cFit_name[i_var]=`gawk 'BEGIN{FS=" ";RS="\n";ORS=""};{print $1}' temp_for4cFit_ivar`
          array_par_4cFit_mass[i_var]=`gawk 'BEGIN{FS=" ";RS="\n";ORS=""};{print $2}' temp_for4cFit_ivar`
          rm temp_for4cFit_ivar
        done
        rm temp_for4cFit
        for (( i_var=0; i_var<num_var; i_var++ ))
        do
          if [ -n "${array_par_4cFit_mass[i_var]}" ]; then
            echo "$indent_forIline""fit_4c_"$auto_gen_part_3"->AddTrack( "$i_var", "${array_par_4cFit_mass[i_var]}", "${array_par_4cFit_name[i_var]}" );" >> temp_forIline_insert
          else
            echo "$indent_forIline""fit_4c_"$auto_gen_part_3"->AddTrack( "$i_var", "${array_par_4cFit_name[i_var]}" );" >> temp_forIline_insert
          fi
        done
        echo "$indent_forIline""fit_4c_"$auto_gen_part_3"->AddFourMomentum( 0, vLor_cms );" >> temp_forIline_insert
        echo "$indent_forIline""bool status_fit_4c_"$auto_gen_part_3" = fit_4c_"$auto_gen_part_3"->Fit();" >> temp_forIline_insert
        for (( i_var=0; i_var<num_var; i_var++ ))
        do
          echo "$indent_forIline""HepLorentzVector vLor_fit_4c_"$auto_gen_part_3"_"$i_var";" >> temp_forIline_insert
        done
        echo "$indent_forIline""double fit_4c_"$auto_gen_part_3"_chisq = 9999;" >> temp_forIline_insert
        echo "$indent_forIline""if( status_fit_4c_"$auto_gen_part_3" ){" >> temp_forIline_insert
        for (( i_var=0; i_var<num_var; i_var++ ))
        do
          echo "$indent_forIline""  vLor_fit_4c_"$auto_gen_part_3"_"$i_var" = fit_4c_"$auto_gen_part_3" -> pfit( "$i_var" );" >> temp_forIline_insert
        done
        echo "$indent_forIline""  fit_4c_"$auto_gen_part_3"_chisq = fit_4c_"$auto_gen_part_3" -> chisq();" >> temp_forIline_insert
        echo "$indent_forIline""}" >> temp_forIline_insert
      else
        cat auto_gen_fit_"$auto_gen_part_2".txt > temp_forFit
        sed -i "s/^\(.\)\(.*\)$/$indent_forIline\1\2/g" temp_forFit
        cat temp_forFit >> temp_forIline_insert
        rm temp_forFit
      fi
    fi
#if pid
    if [[ $auto_gen_part_1 == "pid" ]]; then
      if [ $auto_gen_part_2 == "K" -o $auto_gen_part_2 == "notK" ]; then
        cat $auto_gen_dir"/auto_gen_pid_K.txt" > temp_forPid
        sed -i "s/^\(.\)\(.*\)$/$indent_forIline\1\2/g" temp_forPid
        cat temp_forPid >> temp_forIline_insert
        rm temp_forPid
      fi
      if [[ $auto_gen_part_2 == "notK" ]]; then
        echo "$indent_forIline""if ( !( pid->IsPidInfoValid() ) ){" >> temp_forIline_insert
        echo "$indent_forIline""  "$auto_gen_part_3" = false;" >> temp_forIline_insert
        echo "$indent_forIline""}" >> temp_forIline_insert
        echo "$indent_forIline""if ( pid->probKaon() < pid->probPion() ){" >> temp_forIline_insert
        echo "$indent_forIline""  "$auto_gen_part_3" = false;" >> temp_forIline_insert
        echo "$indent_forIline""}" >> temp_forIline_insert
      fi 
    fi
#if property
    if [ $switch_func_3_property == 1 ]; then
      if [[ $auto_gen_part_1 == "property" ]]; then
        for (( iter_property=0; iter_property<num_property; iter_property++ ))
        do
          echo "$indent_forIline""declareProperty( \""${array_property[iter_property]}"\", "${array_property[iter_property]}" = "${array_property_value[iter_property]}" );" >> temp_forIline_insert
        done
      fi
    fi
#if tuple
    if [ $switch_func_1_tuple == 1 ]; then
      if [[ $auto_gen_part_1 == "tuple" ]]; then
        if [[ $auto_gen_part_2 == "initialize" ]]; then
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
              if [[ ${array_branch_kind[iter_branch]} == "Array" ]]; then
                if [ ${array_branch_is_fixed[iter_branch]} == 0 ]; then
                  echo "$indent_forIline""    status = m_tuple_"${array_tuple[iter_tuple]}"->addItem ( \""${array_branch[iter_branch]}"\", m_"${array_tuple[iter_tuple]}"_"${array_branch_size[iter_branch]}", m_"${array_tuple[iter_tuple]}"_"${array_branch[iter_branch]}" );" >> temp_forIline_insert
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
        if [[ $auto_gen_part_2 == "variableDeclare" ]]; then
          for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
          do
            if [[ ${array_branch_kind[iter_branch]} == "Item" ]];then
              echo "$indent_forIline"""${array_branch_type[iter_branch]}" "${array_branch_belong[iter_branch]}"_"${array_branch[iter_branch]}" = 0;" >> temp_forIline_insert
            fi
            if [[ ${array_branch_kind[iter_branch]} == "Array" ]];then
              if [ ${array_branch_var_as_array[iter_branch]} == 1 ];then
                echo "$indent_forIline"""${array_branch_type[iter_branch]}" "${array_branch_belong[iter_branch]}"_"${array_branch[iter_branch]}"["${array_branch_size[iter_branch]}"];" >> temp_forIline_insert
              fi
              if [ ${array_branch_var_as_array[iter_branch]} == 0 ];then
                echo "$indent_forIline""vector<"${array_branch_type[iter_branch]}"> "${array_branch_belong[iter_branch]}"_"${array_branch[iter_branch]}";" >> temp_forIline_insert
              fi
            fi
          done
        fi
        if [[ $auto_gen_part_2 == "write" ]]; then
          # you'd better check auto_gen_part_3
          for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
          do
            if [ ${array_branch_belong[iter_branch]} != $auto_gen_part_3 ]; then
              continue
            fi
            if [[ ${array_branch_kind[iter_branch]} == "Item" ]];then
              echo "$indent_forIline""m_"$auto_gen_part_3"_"${array_branch[iter_branch]}" = "$auto_gen_part_3"_"${array_branch[iter_branch]}";" >> temp_forIline_insert
            fi
            if [[ ${array_branch_kind[iter_branch]} == "Array" ]];then
              if [ ${array_branch_is_fixed[iter_branch]} == 1 ];then
                echo "$indent_forIline""for ( int i = 0; i < "${array_branch_size[iter_branch]}"; i++ ){" >> temp_forIline_insert
                echo "$indent_forIline""  m_"$auto_gen_part_3"_"${array_branch[iter_branch]}"[i] = "$auto_gen_part_3"_"${array_branch[iter_branch]}"[i];" >> temp_forIline_insert
                echo "$indent_forIline""}" >> temp_forIline_insert
              fi
              if [ ${array_branch_is_fixed[iter_branch]} == 0 ];then
                echo "$indent_forIline""for ( int i = 0; i < "${array_branch_belong[iter_branch]}"_"${array_branch_size[iter_branch]}"; i++ ){" >> temp_forIline_insert
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
        if [[  $auto_gen_part_2 == "getValue" ]]; then
          # you'd better check auto_gen_part_3
          for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
          do
            if [ ${array_branch_belong[iter_branch]} == $auto_gen_part_3 ]; then
              if [ ${array_branch_step[iter_branch]} == $auto_gen_part_4 ]; then
                echo "$indent_forIline"$auto_gen_part_3"_"${array_branch[iter_branch]}" = ""${array_branch_value[iter_branch]}"";" >> temp_forIline_insert
              fi
            fi
          done
        fi
      fi
    fi
#if svc
    if [ $switch_func_2_svc == 1 ]; then
      if [[ $auto_gen_part_1 == "svc" ]]; then
        if [[ $auto_gen_part_2 == "Ncut" ]]; then
          for (( iter_svc=0; iter_svc<num_svc; iter_svc++ ))
          do
            echo "$indent_forIline""int Ncut_"${array_svc[iter_svc]}" = 0;" >> temp_forIline_insert
          done
        fi
        if [[ $auto_gen_part_2 == "declare" ]]; then
          for (( iter_svc=0; iter_svc<num_svc; iter_svc++ ))
          do
            if [[ ${array_svc_classI[iter_svc]} == "SmartDataPtr" ]]; then
              if [[ ${array_svc_classII[iter_svc]} == "pSer_path" ]]; then
                if [[ ${array_svc_namespace[iter_svc]} == "Null" ]]; then
                  echo "$indent_forIline""SmartDataPtr<"${array_svc[iter_svc]}"> ptr_"${array_svc[iter_svc]}"( "${array_svc_pService[iter_svc]}"(), \"/"${array_svc_path[iter_svc]}"\" );" >> temp_forIline_insert
                else
                  echo "$indent_forIline""SmartDataPtr<"${array_svc_namespace[iter_svc]}"::"${array_svc[iter_svc]}"> ptr_"${array_svc[iter_svc]}"( "${array_svc_pService[iter_svc]}"(), \"/"${array_svc_path[iter_svc]}"\" );" >> temp_forIline_insert
                fi
              fi
              if [[ ${array_svc_classII[iter_svc]} == "pSer_pDir" ]]; then
                if [[ ${array_svc_namespace[iter_svc]} == "Null" ]]; then
                  echo "$indent_forIline""SmartDataPtr<"${array_svc[iter_svc]}"> ptr_"${array_svc[iter_svc]}"( "${array_svc_pService[iter_svc]}"(), "${array_svc_pDir[iter_svc]}"::"${array_svc[iter_svc]}" );" >> temp_forIline_insert
                else
                  echo "$indent_forIline""SmartDataPtr<"${array_svc_namespace[iter_svc]}"::"${array_svc[iter_svc]}"> ptr_"${array_svc[iter_svc]}"( "${array_svc_pService[iter_svc]}"(), "${array_svc_pDir[iter_svc]}"::"${array_svc[iter_svc]}" );" >> temp_forIline_insert
                fi
              fi
            fi
            if [[ ${array_svc_classI[iter_svc]} == "other" ]]; then
              sed -i "s/^\(.\)\(.*\)$/$indent_forIline\1\2/g" temp_svcDescribe_${array_svc[iter_svc]}
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
        if [[ $auto_gen_part_2 == "cout" ]]; then
          for (( iter_svc=0; iter_svc<num_svc; iter_svc++ ))
          do
            echo "$indent_forIline""std::cout<<\""${array_svc[iter_svc]}" succeeded = \"<<Ncut_"${array_svc[iter_svc]}"<<std::endl;" >> temp_forIline_insert
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
for (( iter_branch=0; iter_branch<num_branch; iter_branch++ ))
do
  sed -i "s/\<${array_branch[iter_branch]}\>/${array_branch_belong[iter_branch]}_${array_branch[iter_branch]}/g" $head_name_new
done
num_line_head=`gawk '{i++};END{print i}' $head_name_new`
for (( iline=1; iline<=num_line_head; iline++ ))
do 
  sed -n "$iline s/\(.*\)/\1/p" $head_name_new > temp
  if grep -P -q '^[ \t]*//auto_gen' temp ; then
    indent_forIline=`sed -n "s/\(.*\)\/\/auto_gen\(.*\)/\1/p" temp`
    auto_gen_part_1=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $3}' temp`
    auto_gen_part_2=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $4}' temp`
    auto_gen_part_3=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $5}' temp`
    auto_gen_part_4=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $6}' temp`
    touch temp_forIline_insert
#if tuple
    if [ $switch_func_1_tuple == 1 ]; then
      if [[ $auto_gen_part_1 == "tuple" ]]; then
        if [[ $auto_gen_part_2 == "declare" ]]; then
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
#if property
    if [ $switch_func_3_property == 1 ]; then
      if [[ $auto_gen_part_1 == "property" ]]; then
        for (( iter_property=0; iter_property<num_property; iter_property++ ))
        do
          echo "$indent_forIline"${array_property_type[iter_property]}" "${array_property[iter_property]}";" >> temp_forIline_insert
        done
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
#**********************************************modify jobOption**********************************************
cat $jobOption_name > $jobOption_name_new
num_line_jobOption=`gawk '{i++};END{print i}' $jobOption_name_new`
for (( iline=1; iline<=num_line_jobOption; iline++ ))
do 
  sed -n "$iline s/\(.*\)/\1/p" $jobOption_name_new > temp
  if grep -P -q '^[ \t]*//auto_gen' temp ; then
    indent_forIline=`sed -n "s/\(.*\)\/\/auto_gen\(.*\)/\1/p" temp`
    auto_gen_part_1=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $3}' temp`
    auto_gen_part_2=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $4}' temp`
    auto_gen_part_3=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $5}' temp`
    auto_gen_part_4=`gawk 'BEGIN{FS="_";RS="\n";ORS=""};{print $6}' temp`
    touch temp_forIline_insert
#if property
    if [ $switch_func_3_property == 1 ]; then
      if [[ $auto_gen_part_1 == "property" ]]; then
        for (( iter_property=0; iter_property<num_property; iter_property++ ))
        do
          echo "$indent_forIline"$algorithm_name"."${array_property[iter_property]}" = "${array_property_value[iter_property]}";" >> temp_forIline_insert
        done
      fi
    fi
#modify target file
    gawk -v gawk_iline=$iline 'BEGIN{RS="\n";ORS="\n";i=1};{if(i<gawk_iline)print;i++}' $jobOption_name_new > temp_forIline_1
    gawk -v gawk_iline=$iline 'BEGIN{RS="\n";ORS="\n";i=1};{if(i>gawk_iline)print;i++}' $jobOption_name_new > temp_forIline_2
    gawk '//' temp_forIline_1 temp_forIline_insert temp_forIline_2 > $jobOption_name_new
    num_line_temp_forIline_insert=`gawk '{i++};END{print i}' temp_forIline_insert`
    (( num_line_jobOption=num_line_jobOption+num_line_temp_forIline_insert-1 ))
    (( iline=iline+num_line_temp_forIline_insert-1 ))
    rm temp_forIline_insert
    rm temp_forIline_1
    rm temp_forIline_2
  fi
  rm temp
done
