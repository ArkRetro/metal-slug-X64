/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a9e4a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a9e4a(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  if ((int)unaff_A6[0xc] < 0) {
    if ((int)unaff_A6[0xc] < 0xe01) {
      unaff_A6[0xc] = unaff_A6[0xc] + 0xe00;
    }
  }
  else if (0xdff < (int)unaff_A6[0xc]) {
    unaff_A6[0xc] = unaff_A6[0xc] + -0xe00;
  }
  unaff_A6[0xd] = unaff_A6[0xd] + -0x1000;
  candidate_143dc();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    if (*(char *)(unaff_A6 + 0x18) != '\0') {
      candidate_4126();
      iVar2 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar2 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
    *(undefined1 *)(unaff_A6 + 0x18) = 1;
    unaff_A6[0xd] = (int)-unaff_A6[0xd] >> 1;
    sVar3 = candidate_29f2();
    *(short *)((int)unaff_A6 + 0x26) = sVar3 + -0x80 + *(short *)((int)unaff_A6 + 0x26);
  }
  return;
}
