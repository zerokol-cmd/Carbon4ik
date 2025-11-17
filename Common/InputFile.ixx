export module InputFile;

import <fstream>;
import <filesystem>;
import <cassert>;

using filePath_t = std::filesystem::path;

export class InputFile
{
public:
	InputFile(const filePath_t& path)
		: path(path)
	{
	}

	void load()
	{
		if (!isLoaded)
			readInputFile();
		isLoaded = true;
	}

	size_t getSize() const {
		checkLoaded();
		return size;
	}

	std::filesystem::path getPath() const {
		checkLoaded();
		return path;
	}

	bool isUtf() const {
		checkLoaded();
		return isUTF;
	}

	const std::string& getSource() const {
		checkLoaded();
		return source;
	}

	std::string getName() const {
		return path.stem().string();
	}

protected:

	void checkLoaded() const {
		assert(isLoaded);
	}

	void fetchFileInfo(std::ifstream& input)
	{
		size_t startPos = 0;
		size_t endPos = input.tellg();
		isUTF = false;

		input.seekg(0);

		// Check for the UTF-8 marker (BOM) and skip it
		char bom[3] = { 0 };
		input.read(bom, 3);
		if (bom[0] == '\xEF' && bom[1] == '\xBB' && bom[2] == '\xBF')
			isUTF = true;
		else
			input.seekg(0);

		startPos = input.tellg();
		size = endPos - startPos;
	}

	void readInputFile()
	{
		std::ifstream input(path, std::ifstream::ate | std::ifstream::binary);
		input.exceptions(std::ios::failbit | std::ios::badbit);

		fetchFileInfo(input);

		source.resize(size);
		input.read(source.data(), size);
		input.close();
	}

	filePath_t path;
	size_t size = 0;
	bool isUTF = false;
	bool isLoaded = false;
	std::string source;
};