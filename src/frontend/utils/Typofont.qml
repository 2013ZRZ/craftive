pragma Singleton
import QtQuick
import md3.Core

QtObject {
    id: typofont
    readonly property font displayLarge: ({
            family: Theme.typography.displayLarge.family,
            pixelSize: Theme.typography.displayLarge.size,
            weight: Theme.typography.displayLarge.weight
        })
    readonly property font displayMedium: ({
            family: Theme.typography.displayMedium.family,
            pixelSize: Theme.typography.displayMedium.size,
            weight: Theme.typography.displayMedium.weight
        })
    readonly property font displaySmall: ({
            family: Theme.typography.displaySmall.family,
            pixelSize: Theme.typography.displaySmall.size,
            weight: Theme.typography.displaySmall.weight
        })
    readonly property font headlineLarge: ({
            family: Theme.typography.headlineLarge.family,
            pixelSize: Theme.typography.headlineLarge.size,
            weight: Theme.typography.headlineLarge.weight
        })
    readonly property font headlineMedium: ({
            family: Theme.typography.headlineMedium.family,
            pixelSize: Theme.typography.headlineMedium.size,
            weight: Theme.typography.headlineMedium.weight
        })
    readonly property font headlineSmall: ({
            family: Theme.typography.headlineSmall.family,
            pixelSize: Theme.typography.headlineSmall.size,
            weight: Theme.typography.headlineSmall.weight
        })
    readonly property font titleLarge: ({
            family: Theme.typography.titleLarge.family,
            pixelSize: Theme.typography.titleLarge.size,
            weight: Theme.typography.titleLarge.weight
        })
    readonly property font titleMedium: ({
            family: Theme.typography.titleMedium.family,
            pixelSize: Theme.typography.titleMedium.size,
            weight: Theme.typography.titleMedium.weight
        })
    readonly property font titleSmall: ({
            family: Theme.typography.titleSmall.family,
            pixelSize: Theme.typography.titleSmall.size,
            weight: Theme.typography.titleSmall.weight
        })
    readonly property font labelLarge: ({
            family: Theme.typography.labelLarge.family,
            pixelSize: Theme.typography.labelLarge.size,
            weight: Theme.typography.labelLarge.weight
        })
    readonly property font labelMedium: ({
            family: Theme.typography.labelMedium.family,
            pixelSize: Theme.typography.labelMedium.size,
            weight: Theme.typography.labelMedium.weight
        })
    readonly property font labelSmall: ({
            family: Theme.typography.labelSmall.family,
            pixelSize: Theme.typography.labelSmall.size,
            weight: Theme.typography.labelSmall.weight
        })
    readonly property font bodyLarge: ({
            family: Theme.typography.bodyLarge.family,
            pixelSize: Theme.typography.bodyLarge.size,
            weight: Theme.typography.bodyLarge.weight
        })
    readonly property font bodyMedium: ({
            family: Theme.typography.bodyMedium.family,
            pixelSize: Theme.typography.bodyMedium.size,
            weight: Theme.typography.bodyMedium.weight
        })
    readonly property font bodySmall: ({
            family: Theme.typography.bodySmall.family,
            pixelSize: Theme.typography.bodySmall.size,
            weight: Theme.typography.bodySmall.weight
        })
}
