I confirmed that BS->LoadImage() restriction on SecureBoot mode can bypass by using own PE loader witch is implemented on efitools(http://git.kernel.org/cgit/linux/kernel/git/jejb/efitools.git).
This is sample code to load and execute binary without using BS->LoadImage().
