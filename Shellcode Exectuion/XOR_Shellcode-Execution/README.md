# XOR Payload Execution – Windows Defender Detection Experiment

## Overview

This project explores C++ programming, Windows API memory management, XOR-based byte obfuscation and endpoint detection behaviour.

The experiment investigates whether applying a simple XOR transformation to a byte array changes how Microsoft Defender Antivirus detects a payload during execution.

The project was developed in C++ using the Windows API and tested in a controlled Windows environment with Microsoft Defender enabled.

## Objectives

The primary objectives were:

- Understand XOR operations and reversible byte transformations.
- Explore dynamic memory allocation in C++.
- Understand Windows virtual memory management.
- Investigate Windows memory protection mechanisms.
- Examine Windows thread management and synchronisation.
- Observe Microsoft Defender's detection behaviour.
- Compare Defender telemetry between different payload representations.

## Technical Concepts

### 1. XOR Transformation

The program uses a single-byte XOR key (`0x11`) to transform the stored byte array.

XOR is reversible because applying the same key twice restores the original value:

`(A XOR K) XOR K = A`

This project uses XOR as a basic obfuscation technique. It should not be considered cryptographically secure encryption.

### 2. Dynamic Memory Allocation

The program uses C++ dynamic memory allocation to create a buffer for the transformed byte array.

The implementation uses:

- `new[]` to allocate memory.
- A loop to perform the XOR transformation.
- `delete[]` to release the allocated buffer.

### 3. Windows Virtual Memory

The project explores Windows virtual memory through the `VirtualAlloc()` API.

Relevant concepts include:

- `MEM_RESERVE`
- `MEM_COMMIT`
- `PAGE_READWRITE`

It also examines memory protection attributes through `VirtualProtect()`.

### 4. Windows API and Thread Management

The project investigates several Windows API functions:

- `VirtualAlloc()`
- `VirtualProtect()`
- `RtlCopyMemory()`
- `CreateThread()`
- `WaitForSingleObject()`

These provide practical exposure to Windows memory management, API return values, handles and synchronisation.

## Windows Defender Experiment

Two payload representations were compared under the same general Defender configuration.

| Test | Payload representation | Observed result |
|---|---|---|
| 1 | XOR-obfuscated | Payload executed successfully |
| 2 | Unobfuscated | Defender detected and blocked execution |

The XOR-obfuscated version was tested repeatedly. The payload executed successfully without a corresponding Defender detection event appearing in the Windows Defender Operational event log.

The unobfuscated version generated a Defender detection event at approximately 19:08.

## Defender Configuration

Before the subsequent test, the following configuration was recorded:

| Setting | Recorded value |
|---|---|
| Antivirus enabled | True |
| Real-time protection | True |
| Behaviour monitoring | True |
| IOAV protection | True |
| Tamper Protection | True |
| Threat-specific default actions | Empty |

The relevant configuration was obtained using:

`Get-MpComputerStatus`

and:

`Get-MpPreference`

No threat-specific Allow entries were displayed in the recorded configuration.

## Event Log Investigation

The following Windows Defender Operational event IDs were used to investigate detection behaviour:

| Event ID | Description |
|---|---|
| 1116 | Threat detected |
| 1117 | Remediation action taken |
| 1118 | Remediation action failed |
| 5007 | Defender configuration changed |

The unobfuscated test produced a corresponding detection event.

The XOR-obfuscated tests produced no corresponding detection or remediation events in the logs examined.

## Results and Conclusion

The experiment demonstrated a difference in Microsoft Defender's observed detection behaviour between the two payload representations.

The XOR-obfuscated payload executed successfully on multiple occasions, while the unobfuscated version was detected and blocked.

This provides evidence of a repeatable, test-specific Windows Defender detection bypass under the tested configuration.

However, the experiment does not establish that every Defender protection mechanism was bypassed. The absence of a detection event does not independently prove that all scanning and behavioural inspection mechanisms were exercised.

The results are specific to the tested Windows environment, Defender configuration and payload.

## Limitations and Future Investigation

Potential areas for further defensive investigation include:

- Understanding static signature-based detection.
- Investigating behavioural monitoring and process telemetry.
- Examining Windows memory allocation and protection events.
- Comparing Defender detections with Sysmon telemetry.
- Improving C++ memory and handle management.
- Understanding the distinction between successful execution and successful detection prevention.


## Disclaimer

This project was developed for educational cybersecurity research in a controlled environment.

It demonstrates concepts relating to Windows internals, memory management, byte obfuscation and endpoint detection behaviour. It should only be investigated in systems where the researcher has explicit authorisation.
