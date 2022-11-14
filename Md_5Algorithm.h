#pragma once
#include "AlgorithmsBase.h"
#include "IHack.h"
#include "openssl/sha.h"

class Md_5Algorithm : public AlgorithmsBase,
					  public IHack {
public:
	Md_5Algorithm();

	void PasswordToKey() override;
	void Encrypt(const std::string& filePathDest, const std::string& filePathSrc) override;
	void Decrypt(const std::string& filePathDest, const std::string& filePathSrc) override;

	void SetPass(std::string const& fileSrc) override;

protected:
	void CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash) override;
	void EncryptAes(const std::vector<unsigned char> & plainText, std::vector<unsigned char>& chipherText) override;
	void DecryptAes(const std::vector<unsigned char> & chipherText, std::vector<unsigned char>& decryptText) override;

private:
	const EVP_MD* m_dgst;

private:
	void PasswordToKey(std::string& pass);

protected:
	bool CheckPass(const std::vector<unsigned char> & chipherText) override;
	void GetHash(std::vector<unsigned char>& hash, std::string const& fileSrc) const;
	void GetHash(std::vector<unsigned char>& dest, std::vector<unsigned char> const& src) const;
};

