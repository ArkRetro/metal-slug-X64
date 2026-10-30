/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x056e5a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_56e5a(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_1ad6e();
  iVar2 = *(int *)(unaff_A6[3] + 0x78);
  unaff_A6[0x14] = iVar2;
  if (iVar2 != 0) {
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    *unaff_A6 = 0x56e7e;
    if (*(short *)(unaff_A6[3] + 0x4e) != 0) {
      candidate_1aca2();
      candidate_1abc8();
      iVar2 = unaff_A6[3];
      *(undefined1 *)(iVar2 + 0x9a) = 0;
      bVar4 = false;
      if (*(char *)(iVar2 + 0x92) == '\0') {
        candidate_4c24();
        if (bVar4) {
          *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
          bVar3 = *(char *)((int)unaff_A6 + 0x5b) + 1;
          if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
            bVar3 = *(char *)((int)unaff_A6 + 0x5b) + 9;
          }
          if ((bVar3 & 0xf) < 3) {
            *(undefined1 *)(unaff_A6[3] + 0x9a) = 0xff;
          }
        }
      }
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      return;
    }
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
