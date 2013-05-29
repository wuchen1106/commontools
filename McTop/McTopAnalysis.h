#ifndef McTop_Analysis_H
#define McTop_Analysis_H

#include <iostream>
#include <vector>
#include <list>
#include <map>

class McTopAnalysis
{
  public:
    McTopAnalysis();
    ~McTopAnalysis();

    struct TopInfo {
      std::vector<int> mctopList;
      int mctopCounter;
      int mctopMd5[4];

      std::list<int> getFinalStates() const;
      void print( std::ostream& output = std::cout ) const;
      static void printAll( int indexmctop, const int* mctopdata, std::ostream& output = std::cout );
      static void printLine( int dataNum, const int* mctopdata, std::ostream& output = std::cout );
      void latexFinal( std::ostream& output = std::cout ) const;
      void latexTop( std::ostream& output = std::cout ) const;

      bool operator < ( const TopInfo& ti ) const { return mctopCounter > ti.mctopCounter; }
    };

    static bool readPntFile( const std::string& fileName );
    static void readFile( const std::string& fileName, std::ostream& output = std::cout );
    static std::string pdg2Name( int particleId );
    static std::string pdg2LatexName( int particleId );
    static int name2Pdg( const std::string& particleName );

    void add( int mctopNum, const int* mctopData, const int mctopMd5[4] );
    void sort();
    void clear();
    void print( std::ostream& output = std::cout ) const;
    void latex( std::ostream& output = std::cout ) const;

    bool addFinalParticle( int particleId );
    bool addFinalParticle( const std::string& particleName );
    void clearFinalParticle();
    void searchFinalStates();
//    void sortFinalStates();
    void printFinalStates( std::ostream& output = std::cout ) const;
    void latexFinalStates( std::ostream& output = std::cout ) const;

  private:
    void printList( const std::list<TopInfo>& topInfoList, std::ostream& output = std::cout ) const;
    void latexList( const std::list<TopInfo>& topInfoList, std::ostream& output = std::cout ) const;

    static std::map<int, std::string> pdg2NameTab;
    static std::map<int, std::string> pdg2LatexNameTab;
    static std::map<std::string, int> name2PdgTab;

    std::list<TopInfo> m_topInfoList;

    std::list<int> m_finalParticles;
    std::list<TopInfo> m_finalTopList;
};

#endif
