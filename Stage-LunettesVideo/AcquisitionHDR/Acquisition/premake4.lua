-- A solution contains projects, and defines the available configurations
solution "Acquisition_HDR"
	configurations { "Debug", "Release" }

--~ 	dofile "/home/rdeeb/Dev/oasis/PlateformeRA/PlateFormeRAM/PlateFormeRAM/apicamera_v0/build/premake4-lib.lua"
	-- A project defines one build target
	project "CalibrageHDR"
		kind "ConsoleApp"
		language "C++"
		files { "include/Bibliotheque_SVD/*.cpp","src/responseFunctionEstimator.cpp","src/MTB.cpp","src/solveur_Debevec.cpp","mainCalibrage.cpp"}
		includedirs { "include"
								, "/usr/include/OpenEXR"
								, "/usr/include/opencv"
								, "include/Bibliotheque_SVD"
								, "include/Bibliotheque_HDR" }
		
		linkoptions { "-lgtest", 
									"-L../", 
							 		"-L/usr/lib",
						 			"-L./"}
		links { "cv", "highgui", "IlmImf", "IlmThread", "Imath"  }
		buildoptions{"-ggdb -Wall -Wextra" }

	project "HDRfromLDRcollection"
		kind "ConsoleApp"
		language "C++"
		files { "include/Bibliotheque_SVD/*.cpp","src/responseFunctionEstimator.cpp","src/MTB.cpp","src/solveur_Debevec.cpp","mainLdrToHdr.cpp"}
		includedirs { "include"
								, "/usr/include/OpenEXR"
								, "/usr/include/opencv"
								, "include/Bibliotheque_SVD"
								, "include/Bibliotheque_HDR" }
		
		linkoptions { "-L../", 
							 		"-L/usr/lib",
						 			"-L./"}
		links { "cv", "highgui", "IlmImf", "IlmThread", "Imath"  }
		buildoptions{"-ggdb -Wall -Wextra" }

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
		
