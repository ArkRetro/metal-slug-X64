/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01e9f8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1e9f8(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  while( true ) {
    if (*(char *)((int)unaff_A6 + 0x86) < '\x01') {
      candidate_1abc8();
      candidate_49e8();
      *unaff_A6 = candidate_1ea3a;
      bVar3 = false;
      candidate_427a();
      if (bVar3) {
        *unaff_A6 = 0x1e4f2;
      }
      if (0x13f < *(ushort *)(unaff_A6 + 0xf)) {
        *unaff_A6 = 0x1e4f2;
      }
      if (0x13f < (ushort)(*(short *)(unaff_A6 + 0x10) - 0x100U)) {
        *unaff_A6 = 0x1e4f2;
      }
      return;
    }
    candidate_1456a();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) break;
    *(char *)((int)unaff_A6 + 0x86) = *(char *)((int)unaff_A6 + 0x86) + -1;
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
