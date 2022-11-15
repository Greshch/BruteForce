#pragma once
#include "AlgorithmsBase.h"
#include "IHack.h"
#include "PasswordGenerator.h"
#include "openssl/sha.h"

class Md_5Algorithm : public AlgorithmsBase,
					  public IHack {
public:
	Md_5Algorithm();

	void PasswordToKey() override;
	void Encrypt(const std::string& filePathDest, const std::string& filePathSrc) override;
	void Decrypt(const std::string& filePathDest, const std::string& filePathSrc) override;

	void PrepearForHack(std::string const& file);
	
	// Inherited via IHack
	bool SearchPassword(std::string const& file, std::vector<std::string>& balk) override;

protected:
	void CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash) override;
	void EncryptAes(const std::vector<unsigned char> & plainText, std::vector<unsigned char>& chipherText) override;
	void DecryptAes(const std::vector<unsigned char> & chipherText, std::vector<unsigned char>& decryptText) override;
	
	bool CheckPass(const std::vector<unsigned char>& chipherText) override;
	void PasswordToKey(std::string& pass);
	void GetHash(std::vector<unsigned char>& dest, std::vector<unsigned char> const& src) const;

private:
	const EVP_MD* m_dgst;
	std::vector<unsigned char> m_chiferText;
	std::vector<unsigned char> m_originHash;
	std::vector<unsigned char> m_curHash;
	std::vector<unsigned char> m_decryptedText;
};

