#pragma once
#include <forward_list>

#include <DB/IO/WriteBuffer.h>
#include <DB/IO/IReadableWriteBuffer.h>
#include <DB/Core/Defines.h>

namespace DB
{

class ReadBufferFromMemoryWriteBuffer;

/// Stores data in memory chunks, size of cunks are exponentially increasing duting write
/// Data could be reread after write
class MemoryWriteBuffer : public WriteBuffer, public IReadableWriteBuffer
{
public:

	MemoryWriteBuffer(
		size_t max_total_size_ = 0,
		size_t initial_chunk_size_ = DBMS_DEFAULT_BUFFER_SIZE,
		double growth_rate_ = 2.0);

	void nextImpl() override;

	std::shared_ptr<ReadBuffer> getReadBuffer() override;

	~MemoryWriteBuffer() override;

protected:

	const size_t max_total_size;
	const size_t initial_chunk_size;
	const double growth_rate;

	using Container = std::forward_list<BufferBase::Buffer>;

	Container chunk_list;
	Container::iterator chunk_tail;
	size_t total_chunks_size = 0;

	void addChunk();

	friend class ReadBufferFromMemoryWriteBuffer;
};


}