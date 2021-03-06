#include <iostream>
#include <fstream>
#include <sstream>

#include "McTopAnalysis.h"


//////////////////////////////////////////////////////////////////////////////
// member functions for struct TopInfo

std::list<int> McTopAnalysis::TopInfo::getFinalStates() const
{
  std::list<int> tempFinalParticles;
  for( std::vector<int>::size_type i = 0; i < mctopList.size()/2; i++ )
  {
    if( mctopList[i*2+1] == 0 )
    {
      tempFinalParticles.push_back( mctopList[i*2] );
    }
  }
  tempFinalParticles.sort();
  return tempFinalParticles;
}

void McTopAnalysis::TopInfo::print( std::ostream& output/* = std::cout*/ ) const
{
  output << mctopCounter << std::endl;

  printAll( mctopList.size(), &mctopList[0], output );

  for( std::vector<int>::size_type i = 0; i < mctopList.size(); i++ )
  {
    output << mctopList[i] << ", ";
    if( i%2==1 ) output << " ";
  }
  output << std::endl;

  output << "md5: " << mctopMd5[0] << ", " << mctopMd5[1] << ", " << mctopMd5[2] << ", " << mctopMd5[3] << std::endl;

  output << std::endl << std::endl;
}

// old style, not used now
void McTopAnalysis::TopInfo::printLine( int dataNum, const int* mctopdata, std::ostream& output/* = std::cout*/ )
{
  if( dataNum <= 0 )
    return;

  int num = 0;
  for( int i = 0; i < dataNum; i++ )
  {
    output << McTopAnalysis::pdg2Name(mctopdata[i*2]) << " ";
    num += mctopdata[i*2+1];
  }
  output << std::endl;
  printLine( num, mctopdata + dataNum*2, output );
}

void McTopAnalysis::TopInfo::printAll( int indexmctop, const int* mctopdata, std::ostream& output/* = std::cout*/ )
{
  if( indexmctop <= 0 ) {
    output << "No decay list found, check your root file." << std::endl;
    return;
  }

//  printLine( 1, mctopdata, output );

  int daughterPos = 1;
  for( int i = 0; i < indexmctop/2; i++ )
  {
    int particleId = mctopdata[i*2];
    int daughterNum = mctopdata[i*2+1];
    if( daughterNum > 0 )
    {
      output << McTopAnalysis::pdg2Name( particleId ) << " ->";
      for( int j = 0; j < daughterNum; j++ )
      {
        output << " " << McTopAnalysis::pdg2Name( mctopdata[(daughterPos+j)*2] );
      }
      output << std::endl;
    }
    daughterPos += daughterNum;
  }
}

void McTopAnalysis::TopInfo::latexFinal( std::ostream& output/* = std::cout*/ ) const
{
  std::list<int> finalStates = getFinalStates();

  for( std::list<int>::const_iterator cit = finalStates.begin(); cit != finalStates.end(); cit++ )
  {
    output << pdg2LatexName( *cit ) << " ";
  }
}

void McTopAnalysis::TopInfo::latexTop( std::ostream& output/* = std::cout*/ ) const
{
  int daughterPos = 1;
  for( std::vector<int>::size_type i = 0; i < mctopList.size()/2; i++ )
  {
    int particleId = mctopList[i*2];
    int daughterNum = mctopList[i*2+1];
    if( daughterNum > 0 )
    {
      output << McTopAnalysis::pdg2LatexName( particleId ) << " \\rightarrow ";
      for( int j = 0; j < daughterNum; j++ )
      {
        output << McTopAnalysis::pdg2LatexName( mctopList[(daughterPos+j)*2] ) << " ";
      }
      if( daughterPos + daughterNum < int( mctopList.size() / 2 ) )
      {
        output << ", ";
      }
    }
    daughterPos += daughterNum;
  }
}


//////////////////////////////////////////////////////////////////////////////
// static member variables
std::map<int, std::string> McTopAnalysis::pdg2NameTab;
std::map<int, std::string> McTopAnalysis::pdg2LatexNameTab;
std::map<std::string, int> McTopAnalysis::name2PdgTab;

//////////////////////////////////////////////////////////////////////////////
//
McTopAnalysis::McTopAnalysis()
{
}

McTopAnalysis::~McTopAnalysis()
{
}

//////////////////////////////////////////////////////////////////////////////
// static member functions

// convert particle ID to name
std::string McTopAnalysis::pdg2Name( int particleId )
{
  std::map<int, std::string>::const_iterator cit = pdg2NameTab.find( particleId );
  if( cit != pdg2NameTab.end() ) return cit->second;

  // not found and return the particle ID directly
  std::ostringstream oss;
  oss << particleId;
  return oss.str();
}

// convert particle ID to name
std::string McTopAnalysis::pdg2LatexName( int particleId )
{
  std::map<int, std::string>::const_iterator cit = pdg2LatexNameTab.find( particleId );
  if( cit != pdg2LatexNameTab.end() ) return cit->second;

  // not found and return the particle ID directly
  std::ostringstream oss;
  oss << particleId;
  return oss.str();
}

// convert particle name to ID
int McTopAnalysis::name2Pdg( const std::string& particleName )
{ 
  std::map<std::string, int>::const_iterator cit = name2PdgTab.find( particleName );
  if( cit != name2PdgTab.end() ) return cit->second;
  return 0;
}

// this function need to be called in advance so that the two functions(pdg2Name and name2Pdg) can work correctly
bool McTopAnalysis::readPntFile( const std::string& fileName )
{
  std::ifstream pntFile( fileName.c_str() ); 
  if( !pntFile.is_open() )
  {
    std::cout << "Can not open pnt file :" << fileName << std::endl;
    return false;
  }

  std::string line;
  int particleId;
  std::string particleName, particleLatexName;
  while( !pntFile.eof() )
  {
    getline( pntFile, line );
    if( line.empty() )continue;
    if( line[0] == '*' )continue;

    std::stringstream liness;
    liness << line;
    liness >> particleId >> particleName >> particleLatexName;
    if( pdg2NameTab.find( particleId ) != pdg2NameTab.end() ||
        pdg2LatexNameTab.find( particleId ) != pdg2LatexNameTab.end() ||
        name2PdgTab.find( particleName ) != name2PdgTab.end() )
    {
      pntFile.close();
      return false;
    }
    pdg2NameTab[particleId] = particleName;
    pdg2LatexNameTab[particleId] = particleLatexName;
    name2PdgTab[particleName] = particleId;
  }

  pntFile.close();
  return true;
}

void McTopAnalysis::printList( const std::list<TopInfo>& topInfoList, std::ostream& output/* = std::cout*/ ) const
{
  int counter = 0;
  for( std::list<TopInfo>::const_iterator cit = topInfoList.begin(); cit != topInfoList.end(); cit++ )
  {
    output << "NO" << counter++ << ": ";
    cit->print( output );
  }
}

void McTopAnalysis::readFile( const std::string& fileName, std::ostream& output/* = std::cout*/ )
{
  std::ifstream inputFile( fileName.c_str() );

  while( inputFile.good() )
  {
    char tempc;
    inputFile.get( tempc );
    if( inputFile.eof() ) break;
    output.put( tempc );
  }
}

void McTopAnalysis::latexList( const std::list<TopInfo>& topInfoList, std::ostream& output/* = std::cout*/ ) const
{
  readFile( "toplatex/head.tex", output );
  readFile( "toplatex/tab_head.tex", output );

  int counter = 0;
  for( std::list<TopInfo>::const_iterator cit = topInfoList.begin(); cit != topInfoList.end(); cit++ )
  {
    output << counter++ << " & $";
    cit->latexTop( output );
    output << "$ & $";
    cit->latexFinal( output );
    output << "$ & " << cit->mctopCounter << " \\\\" << std::endl;
  }

  readFile( "toplatex/tab_tail.tex", output );
  readFile( "toplatex/tail.tex", output );
}


//////////////////////////////////////////////////////////////////////////////
// member functions

void McTopAnalysis::add( int mctopNum, const int* mctopData, const int mctopMd5[4] )
{
  std::list< TopInfo >::iterator it;
  for( it = m_topInfoList.begin(); it != m_topInfoList.end(); it++ )
  {
    // compare size and every data
    if( mctopNum == (int)(it->mctopList.size()) && equal( it->mctopList.begin(), it->mctopList.end(), mctopData ) )
    {
      // if match
      it->mctopCounter++;
      break;
    }
  }
  // if no match, add it to m_topInfoList
  if( it == m_topInfoList.end() )
  {
    TopInfo titemp;
    titemp.mctopList = std::vector<int>( mctopData, mctopData + mctopNum );
    titemp.mctopCounter = 1;
    std::copy( mctopMd5, mctopMd5 + 4, titemp.mctopMd5 );
    m_topInfoList.push_back( titemp );
  }
}

// sort all decay infomation by amount
void McTopAnalysis::sort()
{
  m_topInfoList.sort();
}

// clear for another set of decays
void McTopAnalysis::clear()
{
  m_topInfoList.clear();
}

// output all the decay
void McTopAnalysis::print( std::ostream& output/* = std::cout*/ ) const
{
  printList( m_topInfoList, output );
}

// output decay in latex form
void McTopAnalysis::latex( std::ostream& output/* = std::cout*/ ) const
{
  latexList( m_topInfoList, output );
}


// add final states of decay
bool McTopAnalysis::addFinalParticle( int particleId )
{
  m_finalParticles.push_back( particleId );
  return true;
}

// add final states of decay via particle name
bool McTopAnalysis::addFinalParticle( const std::string& particleName )
{
  int particleId = name2Pdg( particleName );
  if( particleId == 0 )
  {
    std::cout << "Wrong particle name specified!" << std::endl;
    return false;
  }
  return addFinalParticle( particleId );
}

// clear final states for the preparation of a new set of final states
void McTopAnalysis::clearFinalParticle()
{
  m_finalParticles.clear();
}

// search for specified final states in all decays
void McTopAnalysis::searchFinalStates()
{
  m_finalTopList.clear();

  m_finalParticles.sort();

  for( std::list<TopInfo>::const_iterator cit = m_topInfoList.begin(); cit != m_topInfoList.end(); cit++ )
  {
    // store decays which have same final states
    if( cit->getFinalStates() == m_finalParticles )
    {
      m_finalTopList.push_back( *cit );
    }
  }
}

void McTopAnalysis::printFinalStates( std::ostream& output/* = std::cout*/ ) const
{
  printList( m_finalTopList, output );
}

void McTopAnalysis::latexFinalStates( std::ostream& output/* = std::cout*/ ) const
{
  latexList( m_finalTopList, output );
}
