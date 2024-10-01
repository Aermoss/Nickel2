import sys, os, argparse, subprocess

def checkDir(path: str) -> None:
    if not os.path.exists(path):
        subprocess.run(f"mkdir \"{path}\"", shell = True, stderr = subprocess.PIPE)

def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--path", required = True)
    checkDir(parser.parse_args().path)
    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))