# Memory Usage Limits

This document defines the memory usage limits enforced by CI/CD.

## Current Limits

| Resource  | Total Available           | Warning Threshold     | Error Threshold       | Current Usage |
| --------- | ------------------------- | --------------------- | --------------------- | ------------- |
| **RAM**   | 327,680 bytes (320 KB)    | 70% (229,376 bytes)   | 80% (262,144 bytes)   | ~17.6%        |
| **Flash** | 1,441,792 bytes (1.37 MB) | 80% (1,153,434 bytes) | 90% (1,297,613 bytes) | ~59.9%        |

## CI/CD Enforcement

### Build Workflow

- ✅ **Pass**: Memory usage below error threshold
- ⚠️ **Warning**: Memory usage between warning and error threshold
- ❌ **Fail**: Memory usage exceeds error threshold

### Size Check Workflow (PRs only)

- Compares PR memory usage against base branch
- Reports differences in RAM and Flash usage
- Flags significant changes (>1KB RAM or >10KB Flash)

## What Happens When Limits Are Exceeded?

### RAM > 80%

- Build fails in CI
- PR cannot be merged
- Review code for memory leaks or excessive stack usage

### Flash > 90%

- Build fails in CI
- PR cannot be merged
- Consider removing unused features or optimizing code

## Tips for Reducing Memory Usage

### RAM Optimization

1. Reduce FreeRTOS task stack sizes if possible
2. Use heap allocation sparingly
3. Minimize global variables
4. Use `const` and `PROGMEM` for read-only data

### Flash Optimization

1. Remove unused libraries from `platformio.ini`
2. Disable debug symbols in production builds
3. Use compiler optimization flags
4. Remove unused code and features

## Monitoring Memory Usage

### Local Build

```bash
pio run
```

The build output shows current memory usage:

```
RAM:   [==        ]  17.6% (used 57552 bytes from 327680 bytes)
Flash: [======    ]  59.9% (used 864205 bytes from 1441792 bytes)
```

### CI/CD

- Check the "Build Report" in GitHub Actions artifacts
- PR comments show size comparison with base branch
- View badges in README for current usage

## Updating Limits

To modify the limits, edit `.github/workflows/build.yml`:

```yaml
RAM_LIMIT=80      # Change this value
FLASH_LIMIT=90    # Change this value
```

Commit and push the changes. The new limits will apply to all future builds.
