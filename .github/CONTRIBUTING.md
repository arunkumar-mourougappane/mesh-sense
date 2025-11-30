# Contributing to MeshSense

Thank you for your interest in contributing!

## Development Workflow

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/my-feature`
3. Make your changes
4. Test locally: `pio run`
5. Commit your changes: `git commit -am 'Add new feature'`
6. Push to the branch: `git push origin feature/my-feature`
7. Submit a Pull Request

## Pull Request Process

1. **Automated Checks**: All PRs must pass CI/CD checks:

   - ✅ Build must succeed
   - ✅ RAM usage must be < 80%
   - ✅ Flash usage must be < 90%
   - 📊 Size comparison report generated

2. **Code Review**: At least one maintainer review required

3. **Testing**: Test your changes on actual hardware if possible

4. **Documentation**: Update README.md if adding new features

## CI/CD Checks

### Build Workflow
Runs on every push and PR to verify:
- Code compiles successfully
- Memory usage within limits (RAM < 80%, Flash < 90%)
- Firmware artifacts generated

### Size Check Workflow
Runs on PRs to compare:
- RAM usage vs. base branch
- Flash usage vs. base branch
- Highlights significant changes (>1KB RAM or >10KB Flash)

### Artifacts
Build artifacts (firmware.bin, firmware.elf) are retained for 30 days.
Download from: GitHub Actions → Workflow Run → Artifacts

## Memory Usage Guidelines

Please keep memory usage reasonable:

- **RAM**: Target < 70%, hard limit 80%
- **Flash**: Target < 80%, hard limit 90%

See [MEMORY_LIMITS.md](.github/MEMORY_LIMITS.md) for details.

## Code Style

- Follow existing code style
- Use meaningful variable names
- Comment complex logic
- Keep functions focused and small

## Testing Locally

Before submitting a PR:

```bash
# Build the project
pio run

# Check for errors
pio check

# Upload to test on hardware
pio run -t upload
pio device monitor
```

## Reporting Issues

Use GitHub Issues to report:

- Bugs
- Feature requests
- Documentation improvements

Include:

- Description of the issue
- Steps to reproduce
- Expected vs actual behavior
- Hardware/software versions

## Questions?

Open a GitHub Discussion or Issue for any questions!
