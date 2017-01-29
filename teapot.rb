
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

teapot_version "1.0.0"

define_project "Dream Imaging" do |project|
	project.add_author "Samuel Williams"
	project.license = "MIT License"
	
	project.version = "0.1.0"
end

define_target "dream-imaging" do |target|
	target.build do
		source_root = target.package.path + 'source'
		
		copy headers: source_root.glob('Dream/**/*.{h,hpp}')
		
		build static_library: "DreamImaging", source_files: source_root.glob('Dream/**/*.cpp')
	end
	
	target.depends "Library/Dream"
	target.depends "Library/DreamResources"
	
	target.depends "Language/C++11"

	target.depends "Library/png"
	target.depends "Library/jpeg"
	target.depends "Library/webp"
	
	target.depends "Library/Euclid"
	
	target.provides "Library/DreamImaging" do
		append linkflags {install_prefix + "lib/libDreamImaging.a"}
	end
end

define_target "dream-imaging-tests" do |target|
	target.build do
		copy test_assets: target.package.path.glob('test/**/*.{png,jpg,webp}')
		
		run tests: "DreamImaging", source_files: target.package.path.glob('test/Dream/**/*.cpp')
	end
	
	target.depends "Library/DreamImaging"
	target.depends "Library/UnitTest"
	
	target.provides "Test/DreamImaging"
end

define_configuration "dream-imaging" do |configuration|
	configuration.public!
	
	configuration.require "dream"
	configuration.require "png"
	configuration.require "jpeg"
	configuration.require "webp"
end

define_configuration "development" do |configuration|
	configuration[:source] = "https://github.com/kurocha"
	
	configuration.require "platforms"
	configuration.require "build-cmake"
	configuration.require "build-make"
	configuration.require "unit-test"
	
	configuration.require "language-cpp-class"
	
	configuration.require "platforms"
	configuration.require "dream-resources"
	configuration.import "dream-imaging"
end
