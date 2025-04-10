import sys
import hashlib
if __name__ == "__main__":
    print("*** Create file with MD5 username ***")

    md5 = hashlib.md5()


    username = sys.argv[1]
    md5.update(username.encode('utf-8'))
    username_md5 = md5.hexdigest()

    txt = "\t Username: {} => {}".format(username, username_md5);
    print(txt)


