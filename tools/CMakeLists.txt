### CMakeLists.txt --- 
##
######################################################################
## 
### Commentary: Add directories containing main programs
## 
######################################################################

add_subdirectory(scripts)
add_subdirectory(compare)
add_subdirectory(AnalyzeBicrystal)
add_subdirectory(AnalyzePolycrystal)
add_subdirectory(AnalyzePolycrystal_TimeSerie)
add_subdirectory(AnalyzeCSL)
add_subdirectory(CreateOrientedPlane)
add_subdirectory(CreateGB)
add_subdirectory(CreateFacetGB)
add_subdirectory(ComputeDescriptors)
add_subdirectory(BondOriParam)
add_subdirectory(AtomicStrain)
add_subdirectory(SearchLinkedTetrahedra)
add_subdirectory(Compute_D2Min)
add_subdirectory(AdjustStoichiometry)
add_subdirectory(IdentifyGB)
add_subdirectory(SaveNonCSCrystalBondOriParam)
add_subdirectory(AnalyzeBicrystal_AndAtomicStrain)
add_subdirectory(FitAndSaveGMM)
add_subdirectory(GMMClassification)
add_subdirectory(ClusterAnalysis)
add_subdirectory(ReLabelGMM)
add_subdirectory(IdentifyGrains)
add_subdirectory(DBScanClustering)
