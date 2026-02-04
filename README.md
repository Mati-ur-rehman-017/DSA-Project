# News Article Search Engine

A high-performance search engine built from scratch in C++ to index and search **1.8 million news articles** from the NELA-GT dataset with **20ms query latency**.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Dataset](https://img.shields.io/badge/Dataset-NELA--GT-green.svg)
![Articles](https://img.shields.io/badge/Articles-1.8M-orange.svg)
![Latency](https://img.shields.io/badge/Query%20Latency-20ms-brightgreen.svg)

## Features

- **Fast Search**: 20ms average query latency over 1.8M documents
- **Multi-word Queries**: Find articles containing ALL search terms
- **Relevance Ranking**: Results sorted by aggregated term frequency
- **Memory Efficient**: Stores only byte offsets (~O(vocabulary)) instead of full posting lists
- **Disk-Based**: Handles datasets larger than available RAM

## Architecture

```
┌─────────────────┐     ┌──────────────────┐     ┌─────────────────┐
│  NELA-GT JSON   │────▶│  Forward Index   │────▶│ Inverted Index  │
│   (1.8M docs)   │     │  (doc → words)   │     │ (word → docs)   │
└─────────────────┘     └──────────────────┘     └─────────────────┘
                                                          │
                                                          ▼
┌─────────────────┐     ┌──────────────────┐     ┌─────────────────┐
│  Ranked Results │◀────│  Intersection &  │◀────│   User Query    │
│                 │     │     Ranking      │     │                 │
└─────────────────┘     └──────────────────┘     └─────────────────┘
```

## Quick Start

### Prerequisites

- C++17 compatible compiler
- [Snowball Stemmer](https://snowballstem.org/) library (`libstemmer`)

```bash
# Ubuntu/Debian
sudo apt install libstemmer-dev

# macOS
brew install snowball
```

### Build & Run

```bash
# Clone the repository
git clone https://github.com/Shar-jeel-Sajid/DSA-Project.git
cd DSA-Project

# Compile
g++ main.cpp -lstemmer -o search_engine

# Run (requires pre-built index files)
./search_engine
```

### Usage

```
Enter your Query : climate change policy
Total no of articles: 4523
Time taken: 18 milliseconds
```

## How It Works

### 1. Forward Indexing
Parses JSON articles and extracts word frequencies per document:
```
!<doc_id>\<word>:<count>\<word>:<count>...
```

### 2. Inverted Indexing
Transforms to word→document mapping with byte offsets for O(1) lookup:
```
!<word>\<doc_id>:<score>\<doc_id>:<score>...
```

### 3. Search Algorithm
1. Load only byte offsets into memory (not full posting lists)
2. `seekg()` directly to word positions on disk
3. Intersect posting lists using skip-ahead optimization
4. Rank by aggregated term frequency scores

## Key Innovations

| Innovation | Benefit |
|------------|---------|
| **Byte-offset storage** | O(1) random access, minimal RAM usage |
| **Lazy posting list loading** | Only loads data for query terms |
| **Skip-ahead intersection** | Efficiently finds docs with ALL terms |
| **Custom djb2 hash function** | Consistent hashing across builds |
| **Custom file formats** | No JSON/XML parsing overhead |

## Project Structure

```
DSA-Project/
├── main.cpp                 # Entry point & query interface
├── src/
│   ├── forward_index.cpp    # JSON parsing, forward index builder
│   ├── forward_index.hpp
│   ├── inverted_index.cpp   # Inverted index, search algorithms
│   ├── inverted_index.hpp
│   ├── stemming.cpp         # Snowball stemmer integration
│   ├── stemming.hpp
│   └── nlohmann/json.hpp    # JSON library (vendored)
├── ForwardIndex/
│   ├── Index.txt            # Forward index data
│   ├── metadata.txt         # Article titles & URLs
│   └── count.txt            # Document ID counter
├── inverted.txt             # Inverted index with byte offsets
└── stopwords.json           # English stop words (181 words)
```

## Performance

| Metric | Value |
|--------|-------|
| Dataset Size | 1.8M articles |
| Query Latency | ~20ms |
| Index Build | Single-threaded |
| Memory Model | Disk-based with offset caching |

### Comparison with Industry Solutions

| System | Typical Latency | Notes |
|--------|----------------|-------|
| **This Engine** | **20ms** | Custom C++, disk-based |
| Elasticsearch | 10-50ms | Distributed, RAM-cached |
| SQLite FTS5 | 50-200ms | General-purpose |
| PostgreSQL FTS | 100-500ms | Full RDBMS overhead |

## Dataset

This project uses the [NELA-GT dataset](https://dataverse.harvard.edu/dataset.xhtml?persistentId=doi:10.7910/DVN/ULHLCB) — a large-scale news article collection used for misinformation and media bias research.

## Building the Index

To rebuild indexes from raw NELA-GT JSON files:

```cpp
// In main.cpp, uncomment:
forwardIndex("source.json");  // Build forward index
inverted_index("ForwardIndex/Index.txt", mp);  // Build inverted index
```

## Future Improvements

- [ ] Implement stop word filtering (`stopwords.json` ready)
- [ ] Add TF-IDF scoring
- [ ] Parallel index building
- [ ] Web interface
- [ ] Phrase queries with positional indexing

## License

MIT License

