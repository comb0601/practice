#!/usr/bin/env python3
"""
C++ Tutorial 400 Hours - Interactive Example Runner
====================================================
This script provides an interactive interface to browse and run
all code examples from the tutorial course.
"""

import os
import sys
import subprocess
import platform
from pathlib import Path
from typing import List, Dict, Tuple

# ANSI color codes for terminal output
class Colors:
    HEADER = '\033[95m'
    BLUE = '\033[94m'
    CYAN = '\033[96m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

    @staticmethod
    def disable():
        """Disable colors on Windows if not supported"""
        if platform.system() == 'Windows':
            Colors.HEADER = ''
            Colors.BLUE = ''
            Colors.CYAN = ''
            Colors.GREEN = ''
            Colors.YELLOW = ''
            Colors.RED = ''
            Colors.ENDC = ''
            Colors.BOLD = ''
            Colors.UNDERLINE = ''

# Detect OS
IS_WINDOWS = platform.system() == 'Windows'
IS_LINUX = platform.system() == 'Linux'
IS_MAC = platform.system() == 'Darwin'

# Determine build directory
def find_build_dir() -> Path:
    """Find the most recent build directory"""
    script_dir = Path(__file__).parent

    # Possible build directories in order of preference
    candidates = [
        script_dir / f'output-{"windows" if IS_WINDOWS else "linux"}-Release',
        script_dir / f'output-{"windows" if IS_WINDOWS else "linux"}-Debug',
        script_dir / f'build-{"windows" if IS_WINDOWS else "linux"}',
        script_dir / 'build-verify',
        script_dir / 'build',
    ]

    for candidate in candidates:
        bin_dir = candidate / 'bin'
        if bin_dir.exists() and bin_dir.is_dir():
            return bin_dir

    return None

def find_all_executables(bin_dir: Path) -> Dict[str, List[Tuple[str, Path]]]:
    """Find all executables organized by part"""
    executables = {
        'Part1-Fundamentals': [],
        'Part2-MFC-UI': [],
        'Part3-3D-Rendering': [],
        'Part4-Optimization-Advanced': []
    }

    if not bin_dir or not bin_dir.exists():
        return executables

    # Search for executables
    for part_name in executables.keys():
        part_dir = bin_dir / part_name
        if part_dir.exists():
            for item in sorted(part_dir.rglob('*')):
                if item.is_file():
                    # Check if executable
                    if IS_WINDOWS and item.suffix == '.exe':
                        executables[part_name].append((item.stem, item))
                    elif not IS_WINDOWS and os.access(item, os.X_OK):
                        executables[part_name].append((item.name, item))

    # Also check for Part3 and Part4 subdirectories
    for subdir in bin_dir.iterdir():
        if subdir.is_dir():
            # Handle Part3 subdirectories
            if '3DMath' in subdir.name or 'OpenGL' in subdir.name or 'DirectX' in subdir.name or 'Advanced' in subdir.name or 'Modern' in subdir.name:
                for item in sorted(subdir.rglob('*')):
                    if item.is_file():
                        if IS_WINDOWS and item.suffix == '.exe':
                            executables['Part3-3D-Rendering'].append((f"{subdir.name}/{item.stem}", item))
                        elif not IS_WINDOWS and os.access(item, os.X_OK):
                            executables['Part3-3D-Rendering'].append((f"{subdir.name}/{item.name}", item))

    return executables

def print_header():
    """Print the application header"""
    print(f"\n{Colors.BOLD}{Colors.CYAN}{'='*80}{Colors.ENDC}")
    print(f"{Colors.BOLD}{Colors.CYAN}  C++ Tutorial 400 Hours - Interactive Example Runner{Colors.ENDC}")
    print(f"{Colors.BOLD}{Colors.CYAN}{'='*80}{Colors.ENDC}\n")

def print_menu(executables: Dict[str, List[Tuple[str, Path]]]):
    """Print the main menu"""
    print(f"{Colors.BOLD}Available Parts:{Colors.ENDC}\n")

    menu_items = []
    index = 1

    for part_name, exes in executables.items():
        count = len(exes)
        if count > 0:
            display_name = part_name.replace('-', ' ')
            print(f"  {Colors.GREEN}{index}.{Colors.ENDC} {display_name} ({count} examples)")
            menu_items.append((part_name, exes))
            index += 1

    print(f"\n  {Colors.GREEN}{index}.{Colors.ENDC} Build the project")
    print(f"  {Colors.GREEN}{index+1}.{Colors.ENDC} Verify all code")
    print(f"  {Colors.GREEN}0.{Colors.ENDC} Exit\n")

    return menu_items

def show_part_examples(part_name: str, examples: List[Tuple[str, Path]]):
    """Show all examples for a specific part"""
    print(f"\n{Colors.BOLD}{Colors.BLUE}{part_name}{Colors.ENDC}")
    print(f"{Colors.BLUE}{'='*80}{Colors.ENDC}\n")

    print(f"Found {len(examples)} examples:\n")

    # Group by lesson number if possible
    for idx, (name, path) in enumerate(examples, 1):
        print(f"  {Colors.YELLOW}{idx:3d}.{Colors.ENDC} {name}")

    print(f"\n  {Colors.YELLOW}  0.{Colors.ENDC} Back to main menu\n")

def run_executable(exe_path: Path):
    """Run an executable and display its output"""
    print(f"\n{Colors.BOLD}{Colors.GREEN}Running: {exe_path.name}{Colors.ENDC}")
    print(f"{Colors.GREEN}{'='*80}{Colors.ENDC}\n")

    try:
        # Run the executable
        if IS_WINDOWS:
            result = subprocess.run([str(exe_path)], capture_output=False, text=True)
        else:
            result = subprocess.run([str(exe_path)], capture_output=False, text=True)

        print(f"\n{Colors.GREEN}{'='*80}{Colors.ENDC}")
        if result.returncode == 0:
            print(f"{Colors.GREEN}Program completed successfully{Colors.ENDC}")
        else:
            print(f"{Colors.RED}Program exited with code {result.returncode}{Colors.ENDC}")
        print(f"{Colors.GREEN}{'='*80}{Colors.ENDC}\n")

    except KeyboardInterrupt:
        print(f"\n{Colors.YELLOW}Program interrupted by user{Colors.ENDC}\n")
    except Exception as e:
        print(f"\n{Colors.RED}Error running program: {e}{Colors.ENDC}\n")

    input(f"{Colors.CYAN}Press Enter to continue...{Colors.ENDC}")

def build_project():
    """Run the build script"""
    script_dir = Path(__file__).parent

    print(f"\n{Colors.BOLD}{Colors.BLUE}Building Project...{Colors.ENDC}\n")

    try:
        if IS_WINDOWS:
            build_script = script_dir / 'build_all_windows.bat'
            subprocess.run([str(build_script)], check=True)
        else:
            build_script = script_dir / 'build_all_linux.sh'
            subprocess.run(['bash', str(build_script)], check=True)

        print(f"\n{Colors.GREEN}Build completed successfully!{Colors.ENDC}\n")
    except subprocess.CalledProcessError:
        print(f"\n{Colors.RED}Build failed! See errors above.{Colors.ENDC}\n")
    except Exception as e:
        print(f"\n{Colors.RED}Error: {e}{Colors.ENDC}\n")

    input(f"{Colors.CYAN}Press Enter to continue...{Colors.ENDC}")

def verify_code():
    """Run the verification script"""
    script_dir = Path(__file__).parent

    print(f"\n{Colors.BOLD}{Colors.BLUE}Verifying Code...{Colors.ENDC}\n")

    try:
        if IS_WINDOWS:
            verify_script = script_dir / 'verify_all.bat'
            subprocess.run([str(verify_script)], check=True)
        else:
            verify_script = script_dir / 'verify_all.sh'
            subprocess.run(['bash', str(verify_script)], check=True)

        print(f"\n{Colors.GREEN}Verification completed!{Colors.ENDC}\n")
    except subprocess.CalledProcessError:
        print(f"\n{Colors.RED}Verification failed! See errors above.{Colors.ENDC}\n")
    except Exception as e:
        print(f"\n{Colors.RED}Error: {e}{Colors.ENDC}\n")

    input(f"{Colors.CYAN}Press Enter to continue...{Colors.ENDC}")

def get_input(prompt: str) -> str:
    """Get user input with colored prompt"""
    return input(f"{Colors.CYAN}{prompt}{Colors.ENDC}")

def main():
    """Main application loop"""
    # Disable colors on Windows if needed
    if IS_WINDOWS:
        try:
            import colorama
            colorama.init()
        except ImportError:
            Colors.disable()

    # Find build directory
    bin_dir = find_build_dir()

    if not bin_dir:
        print(f"{Colors.RED}Error: No build directory found!{Colors.ENDC}")
        print(f"\nPlease build the project first using:")
        if IS_WINDOWS:
            print("  build_all_windows.bat")
        else:
            print("  ./build_all_linux.sh")
        print("\nOr select option to build from the menu.\n")
        bin_dir = Path(__file__).parent / 'build' / 'bin'  # Default for menu

    while True:
        # Find all executables
        executables = find_all_executables(bin_dir)

        # Print header and menu
        print_header()
        menu_items = print_menu(executables)

        # Get user choice
        try:
            choice = get_input("Select an option: ").strip()

            if choice == '0':
                print(f"\n{Colors.GREEN}Thank you for using C++ Tutorial Example Runner!{Colors.ENDC}\n")
                break

            choice_num = int(choice)

            # Build option
            if choice_num == len(menu_items) + 1:
                build_project()
                bin_dir = find_build_dir()  # Refresh build dir
                continue

            # Verify option
            if choice_num == len(menu_items) + 2:
                verify_code()
                continue

            # Part selection
            if 1 <= choice_num <= len(menu_items):
                part_name, examples = menu_items[choice_num - 1]

                # Show examples for this part
                while True:
                    show_part_examples(part_name, examples)

                    example_choice = get_input("Select example to run (0 to go back): ").strip()

                    if example_choice == '0':
                        break

                    example_num = int(example_choice)

                    if 1 <= example_num <= len(examples):
                        _, exe_path = examples[example_num - 1]
                        run_executable(exe_path)
                    else:
                        print(f"\n{Colors.RED}Invalid selection!{Colors.ENDC}\n")
                        input(f"{Colors.CYAN}Press Enter to continue...{Colors.ENDC}")
            else:
                print(f"\n{Colors.RED}Invalid selection!{Colors.ENDC}\n")
                input(f"{Colors.CYAN}Press Enter to continue...{Colors.ENDC}")

        except ValueError:
            print(f"\n{Colors.RED}Please enter a valid number!{Colors.ENDC}\n")
            input(f"{Colors.CYAN}Press Enter to continue...{Colors.ENDC}")
        except KeyboardInterrupt:
            print(f"\n\n{Colors.YELLOW}Interrupted by user{Colors.ENDC}\n")
            break
        except Exception as e:
            print(f"\n{Colors.RED}Error: {e}{Colors.ENDC}\n")
            input(f"{Colors.CYAN}Press Enter to continue...{Colors.ENDC}")

if __name__ == '__main__':
    main()
