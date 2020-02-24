#include "SnyIOStream.h"
#include "typedefine.h"

#ifdef _WIN32
#  include <io.h>
#  include <direct.h>
#else
#  include <unistd.h>
#  define _access   access
#endif

#include <stdlib.h>
#include <string>
#include "SnyConfig.h"

using namespace std;
SnyIOStream::SnyIOStream(ByteOrder byteOrder) {
  m_sysByteOrder = SnyIOStream::getSysByteOrder();
  m_ByteOrder = byteOrder;
  m_bReadOk = false;
  m_bWriteOk = false;
}

SnyIOStream::~SnyIOStream() {
}

SnyIOStream::ByteOrder SnyIOStream::getSysByteOrder() {
  uint16_t value = 0x0102;
  uint8_t value0 = ((uint8_t *) &value)[0];
  
  ByteOrder byteOrder = SnyIOStream::LittleEndian;
  if (value0 == 0x01)
    byteOrder = SnyIOStream::BigEndian;
  else
    byteOrder = SnyIOStream::LittleEndian;
  
  return byteOrder;
}

void SnyIOStream::setLittleEndian() {
  m_ByteOrder = SnyIOStream::LittleEndian;
}

void SnyIOStream::setBigEndian() {
  m_ByteOrder = SnyIOStream::BigEndian;
}

bool SnyIOStream::isLittleEndian() {
  return (m_ByteOrder == SnyIOStream::LittleEndian);
}

bool SnyIOStream::isBigEndian() {
  return (m_ByteOrder == SnyIOStream::BigEndian);
}

int64_t SnyIOStream::remainSize() const {
  int64_t nPos = pos();
  int64_t nSize = size();
  if ((nPos < 0) || (nSize < 0))
    return 0;
  return (nSize - nPos);
}

char *SnyIOStream::data() {
  return NULL;
}

const char *SnyIOStream::data() const {
  return NULL;
}

int64_t SnyIOStream::write(SnyIOStream &stream, const int64_t size) {
  if (size <= 0)
    return 0;
  
  const char *dataBegin = stream.data();
  if (dataBegin != NULL) {
    int64_t copySize = stream.size() - stream.pos();
    if (size < copySize)
      copySize = size;
    const char *data = dataBegin + stream.pos();
    return write(data, copySize);
  } else {
    char buffer[64 * 1024];
    int64_t dataLeft = size;
    int64_t readSize = 0;
    int64_t writeSize = 0;
    do {
      readSize = stream.read(buffer, dataLeft > 64 * 1024 ? 64 * 1024 : dataLeft);
      dataLeft -= readSize;
      if (readSize > 0) {
        writeSize = write(buffer, readSize);
      } else {
        break;
      }
    } while (dataLeft > 0 && readSize == writeSize);
    return size - dataLeft;
  }
}

bool SnyIOStream::isReadOk() {
  return m_bReadOk;
}

bool SnyIOStream::isWriteOk() {
  return m_bWriteOk;
}

void SnyIOStream::setReadOk(const bool ok) {
  m_bReadOk = ok;
}

void SnyIOStream::setWriteOk(const bool ok) {
  m_bWriteOk = ok;
}

bool SnyIOStream::swapOrder(char *buffer, int64_t offset, int64_t count) {
  if (count <= 1)
    return true;
  
  if ((offset < 0) || (offset >= count))
    return false;
  
  char *pointer = (char *) (buffer + offset);
  
  for (int64_t i = 0; i < count / 2; i++) {
    int n = (int) (count - i - 1);
    
    char temp = pointer[i];
    pointer[i] = pointer[n];
    pointer[n] = temp;
  }
  return true;
}

template<typename T>
SnyIOStream &SnyIOStream::_read(T &v) {
  read((char *) &v, sizeof(v));
  if (m_bReadOk) {
    if ((isBigEndian() && (m_sysByteOrder == SnyIOStream::LittleEndian)) ||
        (isLittleEndian() && (m_sysByteOrder == SnyIOStream::BigEndian))) {
      SnyIOStream::swapOrder((char *) &v, 0, sizeof(v));
    }
  }
  return *this;
}

template<typename T>
SnyIOStream &SnyIOStream::_write(T v) {
  if ((isBigEndian() && (m_sysByteOrder == SnyIOStream::LittleEndian)) ||
      (isLittleEndian() && (m_sysByteOrder == SnyIOStream::BigEndian))) {
    SnyIOStream::swapOrder((char *) &v, 0, sizeof(v));
  }
  write((const char *) &v, sizeof(v));
  return *this;
}

SnyIOStream &SnyIOStream::operator>>(bool &v) {
  char tempC = 0;
  SnyIOStream::_read(tempC);
  v = (tempC ? true : false);
  return *this;
}

SnyIOStream &SnyIOStream::operator>>(int8_t &v) {
  return SnyIOStream::_read(v);
}

SnyIOStream &SnyIOStream::operator>>(uint8_t &v) {
  return SnyIOStream::_read(v);
}

SnyIOStream &SnyIOStream::operator>>(int16_t &v) {
  return SnyIOStream::_read(v);
}

SnyIOStream &SnyIOStream::operator>>(uint16_t &v) {
  return SnyIOStream::_read(v);
}

SnyIOStream &SnyIOStream::operator>>(int32_t &v) {
  return SnyIOStream::_read(v);
}

SnyIOStream &SnyIOStream::operator>>(uint32_t &v) {
  return SnyIOStream::_read(v);
}

SnyIOStream &SnyIOStream::operator>>(int64_t &v) {
  return SnyIOStream::_read(v);
}

SnyIOStream &SnyIOStream::operator>>(uint64_t &v) {
  return SnyIOStream::_read(v);
}

SnyIOStream &SnyIOStream::operator>>(float &v) {
  return SnyIOStream::_read(v);
}

SnyIOStream &SnyIOStream::operator>>(double &v) {
  return SnyIOStream::_read(v);
}

SnyIOStream &SnyIOStream::operator>>(string &v) {
  uint32_t sSize = 0;
  (*this) >> sSize;
  
  char sBuffer[256];
  char *buffer = sBuffer;
  if (sSize > 256) {
    buffer = new char[sSize];
  }
  //memset(buffer, 0, sSize);
  read(buffer, sSize);
  if (isReadOk()) {
    v = string(buffer, sSize);
  }
  if (buffer != sBuffer) {
    delete[]buffer;
  }
  
  return *this;
}

// write
SnyIOStream &SnyIOStream::operator<<(const bool v) {
  char tempC = (v ? 0xFF : 0);
  SnyIOStream::_write(tempC);
  return *this;
}

SnyIOStream &SnyIOStream::operator<<(const int8_t v) {
  return SnyIOStream::_write(v);
}

SnyIOStream &SnyIOStream::operator<<(const uint8_t v) {
  return SnyIOStream::_write(v);
}

SnyIOStream &SnyIOStream::operator<<(const int16_t v) {
  return SnyIOStream::_write(v);
}

SnyIOStream &SnyIOStream::operator<<(const uint16_t v) {
  return SnyIOStream::_write(v);
}

SnyIOStream &SnyIOStream::operator<<(const int32_t v) {
  return SnyIOStream::_write(v);
}

SnyIOStream &SnyIOStream::operator<<(const uint32_t v) {
  return SnyIOStream::_write(v);
}

SnyIOStream &SnyIOStream::operator<<(const int64_t v) {
  return SnyIOStream::_write(v);
}

SnyIOStream &SnyIOStream::operator<<(const uint64_t v) {
  return SnyIOStream::_write(v);
}

SnyIOStream &SnyIOStream::operator<<(const float v) {
  return SnyIOStream::_write(v);
}

SnyIOStream &SnyIOStream::operator<<(const double v) {
  return SnyIOStream::_write(v);
}

SnyIOStream &SnyIOStream::operator<<(const string &v) {
  uint32_t sSize = (uint32_t) v.size();
  (*this) << sSize;
  write(v.data(), sSize);
  
  return *this;
}

SnyIOStream &SnyIOStream::writeBytes(const uint64_t v, const int size) {
  uint64_t v64 = v;
  for (int i = 0; i < size; i++) {
    uint8_t v8 = (uint8_t) (v64 & 0xFF);
    SnyIOStream::_write(v8);
    v64 >>= 8;
  }
  return *this;
}

SnyIOStream &SnyIOStream::readBytes(uint64_t &v, const int size) {
  v = 0;
  uint8_t v8 = 0;
  for (int i = 0; i < size; i++) {
    v <<= 8;
    SnyIOStream::_read(v8);
    v |= v8;
  }
  return *this;
}

SnyIOStream &SnyIOStream::writeBytes(const uint32_t v, const int size) {
  uint32_t v32 = v;
  for (int i = 0; i < size; i++) {
    uint8_t v8 = (uint8_t) (v32 & 0xFF);
    SnyIOStream::_write(v8);
    v32 >>= 8;
  }
  return *this;
}

SnyIOStream &SnyIOStream::readBytes(uint32_t &v, const int size) {
  v = 0;
  uint8_t v8 = 0;
  uint32_t v32 = 0;
  for (int i = 0; i < size; i++) {
    SnyIOStream::_read(v8);
    v32 = v8;
    v32 <<= (8 * i);
    v |= v32;
  }
  return *this;
}

SnyIOStream &SnyIOStream::readUInt24(int &v) {
  uint8_t v8 = 0;
  uint16_t v16 = 0;
  *this >> v8 >> v16;
  v = ((uint) v8 << 16) | v16;
  return *this;
}

SnyIOStream &SnyIOStream::writeUInt24(const int v) {
  uint16_t v16 = v & 0xFFFF;
  uint8_t v8 = v >> 16;
  *this << v8 << v16;
  return *this;
}

SnyIOStream &SnyIOStream::readInt24(int &v) {
  uint8_t v8 = 0;
  uint16_t v16 = 0;
  *this >> v8 >> v16;
  v = ((int) v8 << 16) | v16;
  return *this;
}

SnyIOStream &SnyIOStream::writeInt24(const int v) {
  uint16_t v16 = v & 0xFFFF;
  int8_t v8 = v >> 16;
  *this << v8 << v16;
  return *this;
}

MemIOStream::MemIOStream(ByteOrder byteOrder)
        : SnyIOStream(byteOrder) {
  m_buffer = NULL;
  m_size = 0;
  m_pos = 0;
  m_bAlloc = false;
  m_maxSize = 0;
}

MemIOStream::MemIOStream(const char *buffer, int64_t size, ByteOrder byteOrder)
        : SnyIOStream(byteOrder) {
  m_buffer = NULL;
  m_size = 0;
  m_maxSize = 0;
  m_pos = 0;
  m_bAlloc = false;
  setBuffer(buffer, size);
}

MemIOStream::MemIOStream(int64_t size, ByteOrder byteOrder)
        : SnyIOStream(byteOrder) {
  m_buffer = NULL;
  m_size = 0;
  m_maxSize = 0;
  m_pos = 0;
  m_bAlloc = false;
  allocSize(size);
}

MemIOStream::MemIOStream(const MemIOStream &s) {
  m_buffer = NULL;
  m_size = s.m_size;
  m_pos = s.m_pos;
  m_bAlloc = false;
  m_maxSize = s.m_maxSize;
  
  if (s.m_buffer != NULL) {
    m_buffer = MemIOStream::allocBufer(m_maxSize);
    if (m_buffer != NULL) {
      memcpy(m_buffer, s.m_buffer, (size_t) m_size);
      m_bAlloc = true;
    }
  }
}

MemIOStream::~MemIOStream() {
  freeBuffer();
}

MemIOStream &MemIOStream::operator=(const MemIOStream &s) {
  freeBuffer();
  
  m_buffer = NULL;
  m_size = s.m_size;
  m_pos = s.m_pos;
  m_bAlloc = false;
  m_maxSize = s.m_maxSize;
  
  if (s.m_buffer != NULL) {
    m_buffer = MemIOStream::allocBufer(m_maxSize);
    if (m_buffer != NULL) {
      memcpy(m_buffer, s.m_buffer, (size_t) m_size);
      m_bAlloc = true;
    }
  }
  
  return (*this);
}

char *MemIOStream::allocBufer(int64_t size) // static
{
  if (size <= 0)
    return NULL;
  return (char *) malloc((size_t) size);
}

char *MemIOStream::reallocBuffer(char *buffer, int64_t size) // static
{
  if ((buffer == NULL) || (size <= 0))
    return NULL;
  return (char *) realloc(buffer, (size_t) size);
}

void MemIOStream::freeBuffer(char *buffer) // static
{
  if (buffer != NULL) {
    free(buffer);
  }
}

void MemIOStream::freeBuffer() {
  if ((m_buffer != NULL) && m_bAlloc) {
    MemIOStream::freeBuffer(m_buffer);
  }
  m_buffer = NULL;
  m_size = 0;
  m_maxSize = 0;
  m_pos = 0;
  m_bAlloc = false;
}

void MemIOStream::allocSize(int64_t size) {
  m_buffer = MemIOStream::allocBufer(size);
  if (m_buffer != NULL) {
    m_size = size;
    m_maxSize = m_size;
    m_pos = 0;
    m_bAlloc = true;
  } else {
    m_buffer = NULL;
    m_size = 0;
    m_maxSize = 0;
    m_pos = 0;
    m_bAlloc = false;
  }
}

void MemIOStream::reallocSize(int64_t size) {
  if (m_buffer == NULL) {
    allocSize(size);
    return;
  }
  
  if (!m_bAlloc)
    return;
  
  if (size <= m_maxSize) {
    if (size > m_size) {
      m_size = size;
    }
    return;
  }
  
  char *pOldBuffer = m_buffer;
  int64_t maxSize = size + 256;
  maxSize += (maxSize >> 3);
  if (maxSize < 4096) {
    maxSize = (maxSize + 255) & 0xFFFFFFFFFFFFFF00LL;
  } else {
    maxSize = (maxSize + 4095) & 0xFFFFFFFFFFFFFC00LL;
  }
  //maxSize = size + 512;
  m_buffer = MemIOStream::reallocBuffer(m_buffer, maxSize);
  if (m_buffer == NULL) {
    MemIOStream::freeBuffer(pOldBuffer);
    m_buffer = NULL;
    m_size = 0;
    m_maxSize = 0;
    m_pos = 0;
    m_bAlloc = false;
  } else {
    m_size = size;
    m_maxSize = maxSize;
  }
}

void MemIOStream::setBuffer(const char *buffer, int64_t size) {
  freeBuffer();
  m_buffer = (char *) buffer;
  m_size = size;
  m_maxSize = size;
  m_bAlloc = false;
}

void MemIOStream::reserve(const int64_t size) {
  int64_t s = m_size;
  reallocSize(size);
  m_size = s;
}

void MemIOStream::resize(const int64_t size) {
  reallocSize(size);
}

void MemIOStream::reset() {
  m_size = 0;
  m_pos = 0;
}

int64_t MemIOStream::read(char *buffer, const int64_t size) {
  if (size <= 0) {
    m_bReadOk = true;
    return 0;
  }
  if ((m_buffer == NULL) || (m_size <= 0)) {
    m_bReadOk = false;
    return -1;
  }
  if ((buffer == NULL) || (size <= 0)) {
    m_bReadOk = false;
    return -1;
  }
  
  if (m_pos >= m_size) {
    m_bReadOk = false;
    return -1;
  }
  
  int64_t readLen = size;
  if (readLen > (m_size - m_pos))
    readLen = (m_size - m_pos);
  
  memcpy(buffer, m_buffer + m_pos, (size_t) readLen);
  m_pos += readLen;
  
  m_bReadOk = (size == readLen);
  
  return readLen;
}

int64_t MemIOStream::write(const char *buffer, const int64_t size) {
  if (size <= 0) {
    m_bWriteOk = true;
    return 0;
  }
  if ((buffer == NULL) || (size <= 0)) {
    m_bWriteOk = false;
    return -1;
  }
  
  reallocSize(size + m_pos);
  if ((m_buffer == NULL) || ((size + m_pos) > m_size)) {
    m_bWriteOk = false;
    return -1;
  }
  
  memcpy(m_buffer + m_pos, buffer, (size_t) size);
  m_pos += size;
  
  m_bWriteOk = true;
  
  return size;
}

bool MemIOStream::seek(int64_t pos) {
  if (pos < 0 || pos > m_size)
    return false;
  m_pos = pos;
  return true;
}

bool MemIOStream::seekToBegin() {
  m_pos = 0;
  return true;
}

bool MemIOStream::seekToEnd() {
  m_pos = m_size;
  return true;
}

bool MemIOStream::skip(int64_t count) {
  if ((m_buffer == NULL) || (m_size <= 0))
    return false;
  
  int64_t pos = m_pos + count;
  if (pos > m_size) {
    m_pos = m_size;
    return false;
  }
  m_pos = pos;
  return true;
}

int64_t MemIOStream::pos() const {
  if ((m_buffer == NULL) || (m_size <= 0))
    return 0;
  
  return m_pos;
}

int64_t MemIOStream::size() const {
  if ((m_buffer == NULL) || (m_size <= 0))
    return 0;
  return m_size;
}

char *MemIOStream::data() {
  return m_buffer;
}

const char *MemIOStream::data() const {
  return m_buffer;
}

bool MemIOStream::atEnd() const {
  return (m_pos >= m_size);
}

QueueIOStream::QueueIOStream(ByteOrder byteOrder)
        : MemIOStream(byteOrder) {
}

QueueIOStream::QueueIOStream(const QueueIOStream &s)
        : MemIOStream(s) {
}

QueueIOStream::~QueueIOStream() {
}

int64_t QueueIOStream::push(const char *buffer, const int64_t size) {
  if ((buffer == NULL) && (size > 0)) {
    return -1;
  }
  const int64_t endPos = m_size;
  reallocSize(m_size + size);
  if (m_buffer == NULL)
    return -1;
  if (size > 0) {
    memcpy(m_buffer + endPos, buffer, (size_t) size);
  }
  return size;
}

int64_t QueueIOStream::cut(const int64_t size) {
  if (size <= m_size) {
    memmove(m_buffer, m_buffer + size, (size_t) (m_size - size));
    m_size = (m_size - size);
    m_pos = ((m_pos - size) >= 0) ? (m_pos - size) : 0;
  } else {
    m_size = 0;
    m_pos = 0;
  }
  return m_size;
}

int64_t QueueIOStream::cut() {
  return cut(m_pos);
}

QueueIOStream &QueueIOStream::operator=(const QueueIOStream &s) {
  MemIOStream::operator=(s);
  return (*this);
}

FileIOStream::FileIOStream(ByteOrder byteOrder)
        : SnyIOStream(byteOrder) {
  m_pFile = NULL;
}

FileIOStream::FileIOStream(const char *filename, FileIOStream::OpenMode openMode,
                           ByteOrder byteOrder)
        : SnyIOStream(byteOrder) {
  m_pFile = NULL;
  open(filename, openMode);
}

FileIOStream::~FileIOStream() {
  close();
}

string _openMode2fopenMode(FileIOStream::OpenMode openMode, const char *fileName = NULL) {
  string sMode;
  if ((openMode & FileIOStream::ReadOnly) && !(openMode & FileIOStream::Truncate)) {
    sMode = "rb";
    if (openMode & FileIOStream::WriteOnly) {
      if ((fileName != NULL) && (_access(fileName, 0) != -1))
        sMode = "rb+";
      else
        sMode = "wb+";
    }
  } else if (openMode & FileIOStream::WriteOnly) {
    sMode = "wb";
    if (openMode & FileIOStream::ReadOnly)
      sMode += "+";
  }
  if (openMode & FileIOStream::Append) {
    sMode = "ab";
    if (openMode & FileIOStream::ReadOnly)
      sMode += "+";
  }
  return sMode;
}

bool FileIOStream::open(const char *filename, FileIOStream::OpenMode openMode) {
  close();
  string mode = _openMode2fopenMode(openMode, filename);
  m_fileName = string(filename);
  m_pFile = SNY_fopen(filename, mode.c_str());
  return isOpen();
}

const string &FileIOStream::getFileName() {
  return m_fileName;
}

bool FileIOStream::isOpen() const {
  return (m_pFile != NULL);
}

void FileIOStream::close() {
  if (m_pFile != NULL) {
    fclose(m_pFile);
    m_pFile = NULL;
  }
  m_fileName = "";
}

bool FileIOStream::flush() {
  if (m_pFile == NULL)
    return false;
  return (fflush(m_pFile) == 0);
}

int64_t FileIOStream::read(char *buffer, const int64_t size) {
  if (m_pFile == NULL) {
    m_bReadOk = false;
    return -1;
  }
  if (size <= 0) {
    m_bReadOk = true;
    return 0;
  }
  int64_t readLen = fread(buffer, 1, (size_t) size, m_pFile);
  m_bReadOk = (readLen == size);
  return readLen;
}

int64_t FileIOStream::write(const char *buffer, const int64_t size) {
  if (m_pFile == NULL) {
    m_bWriteOk = false;
    return -1;
  }
  if (size <= 0) {
    m_bWriteOk = true;
    return 0;
  }
  int64_t writeLen = fwrite(buffer, 1, (size_t) size, m_pFile);
  m_bWriteOk = (writeLen == size);
  return writeLen;
}

int64_t FileIOStream::fseek64Inner(FILE *stream, int64_t offset, int whence) const {
  if (stream == NULL)
    return -1;
  
  if (feof(stream)) {
    rewind(stream);
  } else {
    setbuf(stream, NULL);
  }
  
  int64_t curpos = SNY_fseek(stream, offset, whence);

  if (curpos < 0) {
    return -1;
  }
  return curpos;
}

int64_t FileIOStream::ftell64Inner(FILE *stream) const {
  if (stream == NULL)
    return -1;
  int64_t curPos = fseek64Inner(stream, 0, SEEK_CUR);
  if (curPos >= 0) {
    return curPos;
  } else {
    return -1;
  }
}

bool FileIOStream::seek(const int64_t pos) {
  if (m_pFile == NULL)
    return false;
  
  return (fseek64Inner(m_pFile, pos, SEEK_SET) >= 0);
}

bool FileIOStream::seekToBegin() {
  if (m_pFile == NULL)
    return false;
  
  return (fseek64Inner(m_pFile, 0, SEEK_SET) >= 0);
}

bool FileIOStream::seekToEnd() {
  if (m_pFile == NULL)
    return false;
  
  return (fseek64Inner(m_pFile, 0, SEEK_END) >= 0);
}

bool FileIOStream::skip(int64_t count) {
  if (m_pFile == NULL)
    return false;
  
  int64_t curpos = ftell64Inner(m_pFile);
  return (fseek64Inner(m_pFile, (curpos + count), SEEK_SET) >= 0);
}

int64_t FileIOStream::pos() const {
  if (m_pFile == NULL)
    return -1;
  
  return ftell64Inner(m_pFile);
}

int64_t FileIOStream::size() const {
  if (m_pFile == NULL)
    return 0;
  
  int64_t curpos = ftell64Inner(m_pFile);
  int64_t filesize = fseek64Inner(m_pFile, 0, SEEK_END);
  if (filesize < 0) {
    return -1;
  }
  
  if (fseek64Inner(m_pFile, curpos, SEEK_SET) < 0)
    return -1;
  
  return filesize;
}

bool FileIOStream::atEnd() const {
  if (m_pFile == NULL)
    return true;
  
  int64_t curPos = pos();
  if (curPos < 0)
    return true;
  int64_t fileSize = size();
  if (fileSize < 0)
    return true;
  
  return (curPos >= fileSize);
}


ReversedMemIOStream::ReversedMemIOStream(ByteOrder byteOrder)
        : MemIOStream(byteOrder) {
  m_buffer = NULL;
  m_size = 0;
  m_pos = 0;
  m_bAlloc = false;
  m_maxSize = 0;
}

ReversedMemIOStream::ReversedMemIOStream(const char *buffer, int64_t size, ByteOrder byteOrder)
        : MemIOStream(byteOrder) {
  m_buffer = NULL;
  m_size = 0;
  m_maxSize = 0;
  m_leftEnd = 0;
  m_pos = size;
  m_bAlloc = false;
  setBuffer(buffer, size);
}

ReversedMemIOStream::ReversedMemIOStream(int64_t size, ByteOrder byteOrder)
        : MemIOStream(byteOrder) {
  m_buffer = NULL;
  m_size = 0;
  m_maxSize = 0;
  m_leftEnd = 0;
  m_pos = size;
  m_bAlloc = false;
  allocSize(size);
}

ReversedMemIOStream::ReversedMemIOStream(const MemIOStream &s) {
  m_buffer = NULL;
  m_size = s.size();
  m_leftEnd = s.pos();
  m_pos = s.size();
  m_bAlloc = false;
  m_maxSize = s.size();
  
  if (s.data() != NULL) {
    m_buffer = ReversedMemIOStream::allocBufer(m_maxSize);
    if (m_buffer != NULL) {
      memcpy(m_buffer, s.data(), (size_t) m_size);
      m_bAlloc = true;
    }
  }
}

ReversedMemIOStream::ReversedMemIOStream(const ReversedMemIOStream &s) {
  m_buffer = NULL;
  m_size = s.m_size;
  m_pos = s.m_pos;
  m_bAlloc = false;
  m_maxSize = s.m_maxSize;
  
  if (s.m_buffer != NULL) {
    m_buffer = ReversedMemIOStream::allocBufer(m_maxSize);
    if (m_buffer != NULL) {
      memcpy(m_buffer, s.m_buffer, (size_t) m_size);
      m_bAlloc = true;
    }
  }
}

ReversedMemIOStream::~ReversedMemIOStream() {
  freeBuffer();
}

ReversedMemIOStream &ReversedMemIOStream::operator=(const MemIOStream &s) {
  freeBuffer();
  
  m_buffer = NULL;
  m_size = s.size();
  m_pos = s.size();
  m_bAlloc = false;
  m_maxSize = s.size();
  
  if (s.data() != NULL) {
    m_buffer = ReversedMemIOStream::allocBufer(m_maxSize);
    if (m_buffer != NULL) {
      memcpy(m_buffer, s.data(), (size_t) m_size);
      m_bAlloc = true;
    }
  }
  
  return (*this);
}

ReversedMemIOStream &ReversedMemIOStream::operator=(const ReversedMemIOStream &s) {
  freeBuffer();
  
  m_buffer = NULL;
  m_size = s.m_size;
  m_pos = s.m_pos;
  m_bAlloc = false;
  m_maxSize = s.m_maxSize;
  
  if (s.m_buffer != NULL) {
    m_buffer = ReversedMemIOStream::allocBufer(m_maxSize);
    if (m_buffer != NULL) {
      memcpy(m_buffer, s.m_buffer, (size_t) m_size);
      m_bAlloc = true;
    }
  }
  
  return (*this);
}

char *ReversedMemIOStream::allocBufer(int64_t size) // static
{
  if (size <= 0)
    return NULL;
  return (char *) malloc((size_t) size);
}

char *ReversedMemIOStream::reallocBuffer(char *buffer, int64_t size) // static
{
  if ((buffer == NULL) || (size <= 0))
    return NULL;
  return (char *) realloc(buffer, (size_t) size);
}

void ReversedMemIOStream::freeBuffer(char *buffer) // static
{
  if (buffer != NULL) {
    free(buffer);
  }
}

void ReversedMemIOStream::freeBuffer() {
  if ((m_buffer != NULL) && m_bAlloc) {
    ReversedMemIOStream::freeBuffer(m_buffer);
  }
  m_buffer = NULL;
  m_size = 0;
  m_maxSize = 0;
  m_leftEnd = 0;
  m_pos = 0;
  m_bAlloc = false;
}

void ReversedMemIOStream::allocSize(int64_t size) {
  m_buffer = ReversedMemIOStream::allocBufer(size);
  if (m_buffer != NULL) {
    m_size = size;
    m_maxSize = m_size;
    m_leftEnd = 0;
    m_pos = 0;
    m_bAlloc = true;
  } else {
    m_buffer = NULL;
    m_size = 0;
    m_maxSize = 0;
    m_leftEnd = 0;
    m_pos = 0;
    m_bAlloc = false;
  }
}

void ReversedMemIOStream::reallocSize(int64_t size) {
  if (m_buffer == NULL) {
    allocSize(size);
    return;
  }
  
  if (!m_bAlloc)
    return;
  
  if (size <= m_maxSize) {
    if (size > m_size) {
      m_size = size;
    }
    return;
  }
  
  char *pOldBuffer = m_buffer;
  int64_t maxSize = size + 256;
  maxSize += (maxSize >> 3);
  if (maxSize < 4096) {
    maxSize = (maxSize + 255) & 0xFFFFFFFFFFFFFF00LL;
  } else {
    maxSize = (maxSize + 4095) & 0xFFFFFFFFFFFFFC00LL;
  }
  //maxSize = size + 512;
  m_buffer = MemIOStream::reallocBuffer(m_buffer, maxSize);
  if (m_buffer == NULL) {
    MemIOStream::freeBuffer(pOldBuffer);
    m_buffer = NULL;
    m_size = 0;
    m_maxSize = 0;
    m_leftEnd = 0;
    m_pos = 0;
    m_bAlloc = false;
  } else {
    m_size = size;
    m_maxSize = maxSize;
  }
}

void ReversedMemIOStream::setBuffer(const char *buffer, int64_t size) {
  freeBuffer();
  m_buffer = (char *) buffer;
  m_size = size;
  m_maxSize = size;
  m_bAlloc = false;
}

void ReversedMemIOStream::reserve(const int64_t size) {
  int64_t s = m_size;
  reallocSize(size);
  m_size = s;
}

void ReversedMemIOStream::resize(const int64_t size) {
  reallocSize(size);
}

void ReversedMemIOStream::reset() {
  m_size = 0;
  m_pos = 0;
  m_leftEnd = 0;
}

int64_t ReversedMemIOStream::read(char *buffer, const int64_t size) {
  if (size <= 0) {
    m_bReadOk = true;
    return 0;
  }
  if ((m_buffer == NULL) || (m_size <= 0)) {
    m_bReadOk = false;
    return -1;
  }
  if ((buffer == NULL) || (size <= 0)) {
    m_bReadOk = false;
    return -1;
  }
  
  if (m_pos <= m_leftEnd) {
    m_bReadOk = false;
    return -1;
  }
  
  int64_t readLen = size;
  if (readLen > (m_pos - m_leftEnd))
    readLen = (m_pos - m_leftEnd);
  
  memcpy(buffer, m_buffer - (size_t) readLen, (size_t) readLen);
  m_pos -= readLen;
  
  m_bReadOk = (size == readLen);
  
  return readLen;
}

int64_t ReversedMemIOStream::write(const char *buffer, const int64_t size) {
  return 0;
}

bool ReversedMemIOStream::seek(int64_t pos) {
  if (pos < 0 || pos > m_size)
    return false;
  m_pos = pos;
  return true;
}

bool ReversedMemIOStream::seekToBegin() {
  m_pos = m_leftEnd;
  return true;
}

bool ReversedMemIOStream::seekToEnd() {
  m_pos = m_size;
  return true;
}

bool ReversedMemIOStream::skip(int64_t count) {
  if ((m_buffer == NULL) || (m_size <= 0))
    return false;
  
  int64_t pos = m_pos - count;
  if (pos < m_leftEnd) {
    m_pos = m_leftEnd;
    return false;
  }
  m_pos = pos;
  return true;
}

int64_t ReversedMemIOStream::pos() const {
  if ((m_buffer == NULL) || (m_size <= 0))
    return 0;
  
  return m_pos;
}

int64_t ReversedMemIOStream::size() const {
  if ((m_buffer == NULL) || (m_size <= 0))
    return 0;
  return m_size;
}

char *ReversedMemIOStream::data() {
  return m_buffer;
}

const char *ReversedMemIOStream::data() const {
  return m_buffer;
}

bool ReversedMemIOStream::atEnd() const {
  return (m_pos <= m_leftEnd);
}
