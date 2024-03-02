import sys, os, datetime

def main(argv):
    filePath, fileContent = argv[1], []
    major, minor, revision, build, stage, prefix = 0, 0, 0, 0, None, None

    for i in [i.replace("\n", "") for i in open(filePath, "r").readlines()]:
        if i.startswith("#define"):
            if i.split(" ")[1].endswith("VERSION_MAJOR"):
                major = int(i.split(" ")[2])
                fileContent.append(i)

            elif i.split(" ")[1].endswith("VERSION_MINOR"):
                minor = int(i.split(" ")[2])
                fileContent.append(i)

            elif i.split(" ")[1].endswith("VERSION_REVISION"):
                revision = int(i.split(" ")[2])
                fileContent.append(i)

            elif i.split(" ")[1].endswith("VERSION_BUILD"):
                build = int(i.split(" ")[2]) + 1
                fileContent.append(" ".join(i.split(" ")[:2] + [str(build)]))

            elif i.split(" ")[1].endswith("VERSION_BUILD_DATE"):
                fileContent.append(" ".join(i.split(" ")[:2] + ["\"" + datetime.datetime.now().strftime("%Y-%m-%d") + "\""]))

            elif i.split(" ")[1].endswith("VERSION_STAGE"):
                stage = i.split(" ")[2]

                for j in ["PRE_ALPHA", "ALPHA", "BETA", "RELEASE_CANDIDATE", "RELEASE"]:
                    if stage.endswith(j):
                        prefix = {"PRE_ALPHA": "pre", "ALPHA": "a", "BETA": "b", "RELEASE_CANDIDATE": "rc", "RELEASE": None}[j]
                        break

                fileContent.append(i)

            elif i.split(" ")[1].endswith("VERSION_STRING"):
                version = str(major) + "." + str(minor) + "." + str(build) + ("" if prefix is None else "-" + prefix) + "." + str(revision)
                fileContent.append(" ".join(i.split(" ")[:2] + ["\"" + version + "\""]))

            else:
                fileContent.append(i)

        else:
            fileContent.append(i)

    with open(filePath, "w") as f:
        f.write("\n".join(fileContent))

    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))