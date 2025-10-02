# SCBE

SCBE (simple compiler back-end) is a compiler backend library written from scratch, inspired by LLVM but much simpler.

## Notice

- SCBE is mainly a learning project.
- It is unfinished, not ready for practical use, and nowhere near production quality.
- It is not supposed to match any other compiler back-end in quality, features, or performance.

## Supported targets

### x86_64

x86_64 supports SysV ABI and Windows ABI, and can emit the following files:
- ELF object files
- COFF object files
- Text assembly files

### AArch64

AArch64 currently only supports text assembly file emission.

