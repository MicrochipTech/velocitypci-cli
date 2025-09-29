import subprocess
import re

def strip_ansi(text):
    ansi_escape = re.compile(r'\x1B[@-_][0-?]*[ -/]*[@-~]')
    return ansi_escape.sub('', text)

def run_cmd(cmd):
    try:
        result = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        return result.stdout
    except Exception as e:
        return f"Error running command: {cmd}\n{e}"

def parse_commands(help_output):
    commands = []
    in_commands = False
    for line in help_output.splitlines():
        if 'sub-commands:' in line or 'sub-commands' in line:
            in_commands = True
            continue
        if in_commands:
            if not line.strip() or line.startswith('See '):
                break
            parts = line.strip().split()
            if parts:
                commands.append(parts[0])
    return commands

def parse_extensions(help_output):
    extensions = []
    in_ext = False
    for line in help_output.splitlines():
        if 'installed extensions:' in line:
            in_ext = True
            continue
        if in_ext:
            if not line.strip() or line.startswith('See '):
                break
            parts = line.strip().split()
            if parts:
                extensions.append(parts[0])
    return extensions

def anchorify(title):
    anchor = title.lower()
    anchor = anchor.replace('.', '')
    anchor = anchor.replace(' ', '-')
    anchor = re.sub(r'[^a-z0-9\-]', '', anchor)
    return anchor

def add_section(md_lines, toc_entries, title, content, level=2):
    anchor = anchorify(title)
    clean_content = strip_ansi(content)
    md_lines.append(f"{'#' * level} {title}\n<a name=\"{anchor}\"></a>\n")
    md_lines.append("```text")
    md_lines.append(clean_content.strip())
    md_lines.append("```\n")
    toc_entries.append((level, title, anchor))


def process_subcommands(md_lines, toc_entries, base_cmd, help_output, level=3, parent_cmds=None, is_extension=False):
    if parent_cmds is None:
        parent_cmds = set()
    if 'sub-commands' in help_output:
        subcommands = parse_commands(help_output)
        for subcmd in subcommands:
            if subcmd == 'version' or subcmd == 'help' or subcmd in parent_cmds:
                continue
            # For extensions, use ./switchtec <ext> help <subcmd>
            if is_extension:
                sub_help_cmd = f"{base_cmd} help {subcmd}"
                section_title = f"{base_cmd.split()[-1]} {subcmd}"  # e.g., mfg ping
            else:
                sub_help_cmd = f"{base_cmd} {subcmd} help"
                section_title = subcmd  # e.g., list, info
            sub_help_output = run_cmd(sub_help_cmd)
            add_section(md_lines, toc_entries, section_title, sub_help_output, level=level)
            # Only recurse if this subcommand actually has further subcommands
            if 'sub-commands' in sub_help_output:
                process_subcommands(
                    md_lines, toc_entries, section_title, sub_help_output, level=level+1, parent_cmds=parent_cmds | {subcmd}, is_extension=is_extension
                )

def generate_toc(toc_entries):
    toc_lines = ["## Table of Contents\n"]
    for level, title, anchor in toc_entries:
        indent = '  ' * (level - 2)
        toc_lines.append(f"{indent}- [{title}](#{anchor})")
    toc_lines.append("")  # Blank line after TOC
    return toc_lines

def main():
    md_lines = []
    toc_entries = []
    md_lines.append("# Switchtec CLI Command Reference\n")

    # Always add top-level help and version sections
    help_output = run_cmd('./switchtec help')
    add_section(md_lines, toc_entries, 'help', help_output, level=2)
    version_output = run_cmd('./switchtec version')
    add_section(md_lines, toc_entries, 'version', version_output, level=2)

    # Parse main commands and extensions
    main_commands = parse_commands(help_output)
    extensions = parse_extensions(help_output)

    # Document main commands (skip help and version as subcommands)
    for cmd in main_commands:
        if cmd in ['help', 'version']:
            continue
        cmd_help_output = run_cmd(f'./switchtec help {cmd}')
        add_section(md_lines, toc_entries, cmd, cmd_help_output, level=2)
        process_subcommands(md_lines, toc_entries, cmd, cmd_help_output, level=3, parent_cmds={cmd}, is_extension=False)

    # Document extensions
    for ext in extensions:
        ext_help_output = run_cmd(f'./switchtec {ext} help')
        add_section(md_lines, toc_entries, ext, ext_help_output, level=2)
        process_subcommands(md_lines, toc_entries, ext, ext_help_output, level=3, parent_cmds={ext}, is_extension=True)

    # Generate TOC and prepend to md_lines
    toc_lines = generate_toc(toc_entries)
    md_lines = md_lines[:1] + toc_lines + md_lines[1:]

    # Save the Markdown file
    with open('switchtec_cli_reference.md', 'w') as f:
        f.write('\n'.join(md_lines))
    print("Markdown file generated: switchtec_cli_reference.md")

if __name__ == "__main__":
    main()

