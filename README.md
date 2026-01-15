# ddos

`ddos` is an operating system development project built from the ground up. It takes an inverted approach: define the user-space API first, implement it against existing systems (macOS/Linux) for development, then build the actual kernel underneath. This methodology allows testing user programs while the kernel is still being developed, with the API layer serving as a stable contract between user space and kernel space.

The project covers core OS fundamentals: disk I/O, resource management, process execution, memory management, and system call interfaces.

The goal is **learning through building**, not production tooling.

---

## Project Goals

- Build a functional operating system by starting from user-space and working down to the kernel
- Understand OS-level abstractions by implementing them: disk I/O, resource management, system calls
- Build intuition around disks, files, flags, and bitmasks
- Practice systems programming fundamentals: resource ownership, lifecycle management, error handling
- Explore dynamic linking (`.so` / `.dylib`) and runtime loading behavior

---

## High-Level Architecture

```
ddos/
├── osapi/          # Operating System API Layer (shared library)
│   ├── osapi.c     # Public API implementation
│   ├── osapi.h     # API contract (stable interface)
│   ├── disk.c      # Disk subsystem implementation
│   ├── disk.h      # Disk internal structures
│   ├── os.h        # OS-specific portability layer
│   └── Makefile    # Builds libosapi.dylib/.so
│
├── command/        # User program (analogous to shell/CLI)
│   ├── command.c   # Main program logic
│   ├── command.h   # Command definitions
│   └── Makefile    # Builds command.com executable
│
├── drives/         # Virtual disk images
│   └── disk.1      # Example disk image file
│
└── README.md       # This file
```

The project is split into **two main layers**:

1. **osapi** – a shared library that provides OS-like services
2. **command** – a user program that links against and uses `osapi`

---

## osapi (Operating System API Layer)

The `osapi` directory builds a shared library (`libosapi.so` / `libosapi.dylib`) that exposes a minimal OS-style interface.

### Responsibilities

- Manage disk attachment state
- Provide disk abstractions (`struct disk`)
- Enforce ownership and lifecycle rules
- Hide implementation details from user programs

---

## The Critical Role of the API Layer

The `osapi` layer is not just abstraction for abstraction's sake—it's the **architectural foundation** that makes the entire project viable.

### Why We Need osapi

**Contract Stability**: By defining a clear API boundary, we can develop and test `command.com` immediately using the host OS (macOS/Linux) for implementation, without waiting to build a complete kernel.

**Implementation Independence**: The user program (`command.com`) never calls POSIX functions directly. It only uses `osapi` functions. This means:

- During development: `osapi` wraps host OS calls
- In production: `osapi` talks directly to custom kernel
- `command.com` code remains **completely unchanged**

**Evolutionary Development**: We can:

1. Prototype functionality quickly on the host OS
2. Understand requirements through real usage
3. Gradually replace host OS calls with custom kernel implementations
4. Test both implementations against the same contract

**Learning Value**: This mirrors real-world OS development:

- POSIX is a contract; implementations vary (Linux, BSD, macOS)
- Windows NT maintains Win32 API while kernel evolves underneath
- Microkernels separate mechanism from policy via clean interfaces

### Key Components

#### disk.c / disk.h

Implements disk-related functionality:

- `dinit()`  
  Initializes global disk state.

- `dattach(int drive)`  
  Attaches a disk identified by a logical drive number.  
  Returns a fully initialized `disk*` on success or `NULL` on failure.

- `ddetach(disk *dd)`  
  Detaches a previously attached disk, updates global state, and releases resources.

Internally, disk attachment state is tracked using **bitmasks**, mirroring how real kernels track resources.

#### osapi.c / osapi.h

Acts as the public-facing API layer:

- Exposes safe, documented entry points
- Shields user code from internal implementation details
- Serves as the boundary between “kernel-like” code and user code

---

## command (User Program)

The `command` directory contains a standalone executable that links against `osapi`.

### Responsibilities

- Consume the OS API correctly
- Treat returned objects as opaque resources
- Never manipulate internal state directly
- Respect ownership rules (attach → use → detach)

This separation enforces a **real OS design principle**:

> User programs do not manage kernel data structures directly.

---

## Build System

Each component has its own `Makefile`.

- `osapi` builds a shared library
- `command` links dynamically against `osapi`
- Runtime linking is handled via `rpath` / `@loader_path`

This setup is intentional to explore:

- Dynamic loaders (`dyld`, `ld.so`)
- Install names and rpaths
- Common shared-library failure modes

---

## Status

This project is **actively evolving** as part of a deeper study into:

- Operating systems
- File systems
- Low-level abstractions
- System interfaces

Expect frequent refactors as understanding deepens.

---

## Author

Omozuwa Omede  
Focus: Systems Programming, Operating Systems, Backend Engineering  
Developed under the guidance of Dr. Jonas Birch.

_Building from first principles to understand how computers really work._
