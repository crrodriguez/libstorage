
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "common.h"


using namespace std;
using namespace storage;


StorageInterface* s = 0;


void
print_fstab ()
{
    ifstream fstab("tmp/etc/fstab");
    string line;

    while (getline (fstab, line))
	cout << line << '\n';
}


void
run1 ()
{
    cout << "run1\n";

    s = createStorageInterface(TestEnvironment());

    string disk = "/dev/hdb";

    s->destroyPartitionTable (disk, "msdos");

    long int S = 4 * 1000000;

    string name;
    cout << s->createPartitionKb (disk, PRIMARY, 0, S, name) << '\n';

    cout << name << '\n';

    cout << s->changeFormatVolume (name, true, REISERFS) << '\n';
    cout << s->changeMountPoint (name, "/tmp/mnt") << '\n';
    cout << s->changeMountBy (name, MOUNTBY_UUID) << '\n';

    cout << s->commit () << '\n';

    delete s;
}


void
run2 ()
{
    cout << "run2\n";

    s = createStorageInterface(TestEnvironment());

    string name = "/dev/hdb1";

    cout << name << '\n';

    cout << s->changeMountBy (name, MOUNTBY_DEVICE) << '\n';

    cout << s->commit () << '\n';

    delete s;
}


int
main()
{
    cout.setf(std::ios::boolalpha);

    setup_logger();

    setup_system();

    system ("cp data/disk_hdb.info tmp");

    run1 ();
    print_fstab ();

    run2 ();
    print_fstab ();
}
