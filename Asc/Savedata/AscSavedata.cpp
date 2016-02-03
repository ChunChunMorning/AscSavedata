# include "../Savedata.hpp"
# include <unordered_map>
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	class Savedata::CSavedata
	{
	public:

		CSavedata() : m_delimiter('@') {}

		CSavedata(const AES128Key& aes128Key, wchar delimiter) : m_delimiter(delimiter), m_aes128Key(aes128Key) {}

		wchar m_delimiter;

		AES128Key m_aes128Key;

		std::unordered_map<String, String> m_data;

		FilePath m_lastPath;
	};
}

using namespace asc;

Savedata::Savedata() : pImpl(std::make_shared<CSavedata>()) {}

Savedata::Savedata(const AES128Key& aes128Key) : pImpl(std::make_shared<CSavedata>(aes128Key, '@')) {}

Savedata::Savedata(const AES128Key& aes128Key, wchar delimiter) : pImpl(std::make_shared<CSavedata>(aes128Key, delimiter)) {}

String Savedata::getString(const String& key) const
{
	return pImpl->m_data.at(key);
}

void Savedata::addString(const String& key, const String& value)
{
	pImpl->m_data[key] = value;
}

bool Savedata::hasKey(const String& key) const
{
	return pImpl->m_data.find(key) != pImpl->m_data.end();
}

size_t Savedata::erase(const String& key)
{
	return pImpl->m_data.erase(key);
}

void Savedata::clear()
{
	pImpl->m_data.clear();
}

bool Savedata::load()
{
	return load(pImpl->m_lastPath);
}

bool Savedata::load(const FilePath& filePath)
{
	BinaryReader reader(filePath);

	if (!reader)
		return false;

	String data;

	if (!Crypto2::DecryptString(reader.readWhole(), data, pImpl->m_aes128Key))
		return false;

	auto contents = data.substr(1).split(data[0]);

	for (size_t i = 0; i < contents.size(); i += 2)
	{
		pImpl->m_data[contents[i]] = contents[i + 1];
	}

	return true;
}

bool Savedata::loadByPlaneText()
{
	return loadByPlaneText(pImpl->m_lastPath);
}

bool Savedata::loadByPlaneText(const FilePath& filePath)
{
	TextReader reader(filePath);
	
	if (!reader)
		return false;
	
	String data = reader.readAll();

	auto contents = data.substr(1).split(data[0]);

	for (size_t i = 0; i < contents.size(); i += 2)
	{
		pImpl->m_data[contents[i]] = contents[i + 1];
	}

	return true;
}

bool Savedata::save()
{
	return save(pImpl->m_lastPath);
}

bool Savedata::save(const FilePath& filePath)
{
	String data = L"";

	for (auto content : pImpl->m_data)
	{
		data += pImpl->m_delimiter + content.first + pImpl->m_delimiter + content.second;
	}

	const auto encrypted = Crypto2::EncryptString(data, pImpl->m_aes128Key);

	return encrypted.save(filePath);
}

bool Savedata::saveByPlaneText()
{
	return saveByPlaneText(pImpl->m_lastPath);
}

bool Savedata::saveByPlaneText(const FilePath& filePath)
{
	String data = L"";

	for (auto content : pImpl->m_data)
	{
		data += pImpl->m_delimiter + content.first + pImpl->m_delimiter + content.second;
	}

	TextWriter writer(filePath);

	if(!writer)
		return false;

	writer.writeln(data);

	return true;
}
