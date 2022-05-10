# NetdataBlock
A balena block to monitor your device with [netdata/netdata](https://github.com/netdata/netdata).

[![deploy button](/deploy.svg)](https://dashboard.balena-cloud.com/deploy?repoUrl=https://github.com/OdysLam/netdatablock)


## Features

![Netdata in
action](https://user-images.githubusercontent.com/1153921/113440964-449c2180-93a2-11eb-9664-663afa1257a8.gif)

Here's what you can expect from Netdata:

-   **1s granularity**: The highest possible resolution for all metrics.
-   **Unlimited metrics**: Netdata collects all the available metrics—the more, the better.
-   **1% CPU utilization of a single core**: It's unbelievably optimized.
-   **A few MB of RAM**: The highly-efficient database engine stores per-second metrics in RAM and then "spills"
    historical metrics to disk long-term storage.
-   **Minimal disk I/O**: While running, Netdata only writes historical metrics and reads `error` and `access` logs.
-   **Zero configuration**: Netdata auto-detects everything, and can collect up to 10,000 metrics per server out of the
    box.
-   **Zero maintenance**: You just run it. Netdata does the rest.
-   **Stunningly fast, interactive visualizations**: The dashboard responds to queries in less than 1ms per metric to
    synchronize charts as you pan through time, zoom in on anomalies, and more.
-   **Visual anomaly detection**: Our UI/UX emphasizes the relationships between charts to help you detect the root
    cause of anomalies.
-   **Scales to infinity**: You can install it on all your servers, containers, VMs, and IoT devices. Metrics are not
    centralized by default, so there is no limit.
-   **Several operating modes**: Autonomous host monitoring (the default), headless data collector, forwarding proxy,
    store and forward proxy, central multi-host monitoring, in all possible configurations. Use different metrics
    retention policies per node and run with or without health monitoring.

Netdata works with tons of applications, notifications platforms, and other time-series databases:

-   **300+ system, container, and application endpoints**: Collectors autodetect metrics from default endpoints and
    immediately visualize them into meaningful charts designed for troubleshooting. See [everything we
    support](https://learn.netdata.cloud/docs/agent/collectors/collectors).
-   **20+ notification platforms**: Netdata's health watchdog sends warning and critical alarms to your [favorite
    platform](https://learn.netdata.cloud/docs/monitor/enable-notifications) to inform you of anomalies just seconds
    after they affect your node.
-   **30+ external time-series databases**: Export resampled metrics as they're collected to other [local- and
    Cloud-based databases](https://learn.netdata.cloud/docs/export/external-databases) for best-in-class
    interoperability.

> 💡 **Want to leverage the monitoring power of Netdata across entire infrastructure**? View metrics from
> any number of distributed nodes in a single interface and unlock even more
> [features](https://learn.netdata.cloud/docs/overview/why-netdata) with [Netdata
> Cloud](https://learn.netdata.cloud/docs/overview/what-is-netdata#netdata-cloud).

