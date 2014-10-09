//
//  libimpkg.h
//  iMods
//
//  Created by Ryan Feng on 9/20/14.
//  Copyright (c) 2014 Ryan Feng. All rights reserved.
//

#ifndef __iMods__libimpkg__
#define __iMods__libimpkg__

#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <fstream>
#include <map>
#include <ostream>

#pragma mark Forward Declarations

class TagSection;
class TagFile;
class FilterCondition;
class Package;
class Version;
class PackageCache;
class DependencySolver;

#pragma mark Enumerations

enum PackageVersionOp {
    VER_EQ = 0,
    VER_LT = -2,
    VER_LE = -1,
    VER_GT = 1,
    VER_GE = 2,
    VER_ANY = 0xFF
};

#pragma mark typedef

typedef std::pair<std::string, std::string> TagField;

typedef std::vector<FilterCondition> FilterConditions;

typedef std::tuple<std::string, PackageVersionOp, std::string> PackageDepTuple;

typedef std::vector<PackageDepTuple> DepVector;

typedef PackageCache PackageIndex;

#pragma mark Constants

// How far the dependency resolver can go in the dependency graph
// Increase its value as the algorithm improves and the number of packages increases
const int MaxDependencyResolveDepth = 100;

#pragma mark Utilities

std::string depTuplePackageName(const PackageDepTuple& dep);

PackageVersionOp depTupleVersionOp(const PackageDepTuple& dep);

std::string depTupleVersion(const PackageDepTuple& dep);

#pragma mark Core Functions

int pkgVersionCmp(const std::string& v1, const std::string& v2);

std::ostream& operator<<(std::ostream& stream, const PackageDepTuple& dep);

size_t calcInstalledSize(TagFile& localCache);

size_t calcDownloadSize(TagFile& remoteIndex, const std::vector<TagSection>& toInstallPackages);

#pragma mark -

/* Each TagSection represents a section in an index file.
 * Usually a section is the control information of a package,
 * but it doesn't necessarily to be one.
 */
class TagSection {
    
public:
    TagSection();
    TagSection(TagSection* next);
    TagSection(const TagSection& other);
    TagSection(const TagSection&& other);
    
    TagSection& operator=(const TagSection& other);
    
    ~TagSection();
    
    TagSection* next();
    void setNext(TagSection* next);
    
    bool tag(const std::string& tagname, std::string& out_tagvalue) const;
    
    TagSection& operator << (const TagField& field);
    
    const std::string& operator [] (const std::string& tagname);
    
    size_t fieldCount() const;

private:
    
    std::unordered_map<std::string, std::string> m_mapping;
    
    TagSection* m_next;
};

#pragma mark -

class FilterCondition {
    
public:
    
    enum FilterOperator {
        TAG_EQ,
        TAG_NE,
        /* Lexical comparison */
        TAG_A_LT,
        TAG_A_GT,
        TAG_A_LE,
        TAG_A_GE,
        /* Numeric comparison */
        TAG_I_LT,
        TAG_I_GT,
        TAG_I_LE,
        TAG_I_GE,
        /* Version comparison */
        TAG_V_LT,
        TAG_V_GT,
        TAG_V_LE,
        TAG_V_GE,
        /* Date and time comparison */
        // Currently not used
        TAG_D_LT,
        TAG_D_GT,
        TAG_D_LE,
        TAG_D_GE
    };
    
    FilterCondition(const TagField& srcField, FilterOperator op);
    
    virtual ~FilterCondition();
    
    virtual bool matchSection(const TagSection& section) const;
    
    virtual bool matchTag(const std::string& tagname, const std::string& tagvalue) const;
    
private:
    
    const TagField m_srcField;
    
    FilterOperator m_op;
};

#pragma mark -

class TagFile {
    
public:
    TagFile(const std::string& filename=std::string());
    
    ~TagFile();
    
    // open a tag file
    bool open(const std::string& filename);
    
    // return current section
    const TagSection& section() const;
    
    // go to next section
    bool nextSection();
    
    size_t sectionCount() const;
    
    void rewind();
    
    // query current section
    bool tag(const std::string& tagname, std::string& out_tagvalue);
    
    // filter all sections and return the ones that meet the conditions
    std::vector<TagSection> filter(const FilterConditions& conds);
    
private:
    void parseSections(std::ifstream& stream);
    std::vector<TagSection> m_sections;
    int m_cur;
};

#pragma mark -

class Package {
    
public:
    
    Package();
    
    Package(const std::string& pkgName, TagFile& ctrlFile, bool installed=false);
    
    Package(const std::string& pkgName, const std::vector<TagSection>& sections, bool installed=false);
    
    Package(const std::string& pkgName, bool installed=false);
    
    Package(const Package& other);

    Package(const Package&& other);
    
    ~Package();
    
    // Currently installed version
    const Version* curVersion() const;
    
    void setCurVersion(const std::string verStr);
    
    const std::string& name() const;
    
    const std::vector<const Version*> ver_list() const;
    
    const Version* checkDep(const PackageDepTuple& dep) const;
    
    void addVersion(const Version& version);
    
    const Version* version(const std::string& version)const;
    
    size_t versionCount() const;
    
    Package& operator=(const Package& other);
    
    Package& operator=(const Package&& other);
    
private:
    
    void initWithTagFile(const std::string& pkgName, TagFile& ctrlFile, bool installed);
    
    void initWithSections(const std::vector<TagSection>& sections, bool installed);
    
    std::string m_pkgName;
    
    const Version* m_curVersion;
    
    std::map<std::string, Version> m_versions;
    
};

class Version {
    
public:

    Version(const TagSection& ctrlFile);
    Version(const Version& other);
    
    ~Version();
    
    uint64_t itemID() const;
    
    // Check whether the version fullfils the dependency
    bool checkDep(const PackageDepTuple& dep) const;
    
    std::string packageName() const;
    
    std::string version() const;
    
    std::string depString () const;

    std::string sha1Checksum() const;
    
    std::string md5Checksum() const;
    
    std::string sha256Checksum() const;
    
    const std::vector<std::vector<PackageDepTuple>>& dep_list() const;
    
    const std::string& debFilePath() const;
    
    void setDebFilePath(const std::string& path);
    
    // Compare versions
    bool operator<(const Version& version);
    
    bool operator>(const Version& version);
    
    bool operator<=(const Version& version);
    
    bool operator>=(const Version& version);
    
    bool operator==(const Version& version);
    
    friend std::ostream& operator<<(std::ostream& out, const Version& version) {
        out << version.packageName() << " " << version.version();
        return out;
    }
    
private:
    
    std::vector<std::vector<PackageDepTuple>> m_depList;
    
    std::string m_debFilePath;
    
    TagSection m_section;
};

class PackageCache {
    
public:
    PackageCache(TagFile& cacheFile);
    PackageCache(const std::string& filename);
    ~PackageCache();
    
    void addPackage(const Package& package);
    
    bool checkDep(const PackageDepTuple& dep) const;
    
    void markInstalled(TagFile& cacheFile);

    void markInstalled(const std::string& tagfilename);
    
    const Package* package(const std::string& pkgName) const;
    
    const Version* findFirstVersionOfDeps(const std::vector<PackageDepTuple>& deps) const;
    
    const std::map<std::string, Package>& allPackages() const;
    
private:
    
    PackageCache(const PackageCache&) {}
    PackageCache& operator=(const PackageCache&) { return *this;}
    
    void initWithTagFile(TagFile& cacheFile);
    
    std::map<std::string, Package> m_packages;
};

#pragma mark -

class DependencySolver {
    
public:
    
    DependencySolver(const PackageCache& cache, const PackageIndex& index,
                     const DepVector& unresolvedDeps);
    
    DependencySolver(const PackageCache& cache, const PackageIndex& index);

    void initUnresolvedDeps(const DepVector& unresolvedDeps);
    
    DepVector getUpdates() const;
    
    DepVector findSolution();
    
    bool calcDep(std::vector<const Version*>& out_targetDeps, DepVector& out_brokenDeps);
    
private:
    
    typedef std::pair<std::string, std::pair<const Version*, PackageDepTuple>> RevDepMapEntry;
    
    // NOTE: We use 'Step' structure here, which is similar to what aptitude used,
    // so we can extend it to a better algorithm in the future.
    struct Step {
        // For root step, parent is nullptr
        Step* parent;
        
        // 'children' stores all steps of the dependencies of current version
        std::vector<Step> children;
        
        int level;
        
        // The current package needs to be resolved
        const Package* curPackage;
        
        // For root step, srcVersion is nullptr, otherwise it's the same as parent->srcVersion
        const Version* srcVersion;
        
        //  The current selected version, it's also the resolved version after all dependencies are fulfilled
        const Version* targetVersion;
        
        PackageDepTuple targetDep;
        
        int curVerIndex;
        
        std::vector<const Version*> versions;
        
        Step(const Package* curPackage, const PackageDepTuple& targetDep, Step* parent=nullptr, const Version* srcVersion=nullptr, const Version* targetVersion=nullptr);
        Step(const Step && other);
        Step(const Step & other);
        Step();
        
        Step& operator=(const Step& other);
        
        friend std::ostream& operator<<(std::ostream& out, const Step& step) {
            out << std::endl;
            out << "Step: " << step.level << std::endl;
            out << "parent: " << step.parent->level << std::endl;
            out << "package: " << step.curPackage->name() << std::endl;
            out << "srcVersion: " << *step.srcVersion << std::endl;
            out << "versions: " << std::endl;
            for (auto version: step.versions) {
                out << " " << *version << std::endl;
            }
            return out;
        }
    };
    
    void addSuccessors(Step& step);
    
    void processSteps();
    
    void processNextVersion(Step& step);
    
    Step& newStep(const Package* curPackage, const PackageDepTuple& targetDep, Step* parent=nullptr, const Version* srcVer=nullptr);
    
    bool resolveSingleDep(Step& step);
    
    std::vector<std::pair<const Version*, PackageDepTuple>> checkConflicts(const Version* targetVersion);
    
    void initRevDepMap();
    
    void clearContainers();
    
    RevDepMapEntry make_revDepEntry(const std::string& targetPkgName, const Version* srcVersion, const PackageDepTuple& targetDep);
    
    std::unordered_set<std::string> m_visited;
    
    // The actual dependency graph
    // A "target package name" to "source step" mapping
    std::unordered_map<std::string, Step> m_steps;
    
    std::queue<Step*> m_unprocessedSteps;
    
    // Reverse dependencies map, <target package name, <source version, target dependency>>
    std::unordered_multimap<std::string, std::pair<const Version*, PackageDepTuple>> m_cacheRevDepMap;
    
    const PackageCache& m_cache;
    
    const PackageIndex& m_index;
    
    DepVector m_unresolvedDeps;

    std::vector<const Version*> m_resolvedDeps;
    
    DepVector m_brokenDeps;
};

#endif /* defined(__iMods__libimpkg__) */