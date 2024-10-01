import sys, argparse, datetime

def incVersion(content: list[str]) -> str:
    result, stages = [], {}
    major, minor, revision, build, prefix = \
        None, None, None, None, None
    
    for i in content.split("\n"):
        if "#define" in i.split(" "):
            match "_".join(i.split(" ")[1].split("_")[1:]):
                case "VERSION_MAJOR":
                    major = int(i.split(" ")[2])

                case "VERSION_MINOR":
                    minor = int(i.split(" ")[2])

                case "VERSION_REVISION":
                    revision = int(i.split(" ")[2])

                case "VERSION_BUILD":
                    build = int(i.split(" ")[2]) + 1
                    i = " ".join(i.split(" ")[:2] + [str(build)])

                case "VERSION_BUILD_DATE":
                    i = " ".join(i.split(" ")[:2] + ["\"" + datetime.datetime.now().strftime("%Y-%m-%d") + "\""])

                case "VERSION_STAGE":
                    prefix = ["pre", "a", "b", "rc", None][stages["_".join(i.split(" ")[2].split("_")[2:])]]

                case "VERSION_STRING":
                    i = " ".join(i.split(" ")[:2] + \
                        ["\"" + ".".join([str(major), str(minor), str(build) + ("-" + prefix if prefix else ""), str(revision)]) + "\""])
                    
                case _:
                    if i.split(" ")[1].split("_")[1] in ["STAGE"]:
                        stages["_".join(i.split(" ")[1].split("_")[2:])] = int(i.split(" ")[2])

        result.append(i)

    return "\n".join(result)

def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--path", required = True)
    args = parser.parse_args()

    with open(args.path, "r") as file:
        result = incVersion(file.read())

    with open(args.path, "w") as file:
        file.write(result)

    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))