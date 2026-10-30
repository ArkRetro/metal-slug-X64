/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x068174.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_68174(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  if ((int)unaff_A6[0xc] < 0) {
    if ((int)unaff_A6[0xc] < 0x1001) {
      unaff_A6[0xc] = unaff_A6[0xc] + 0x1000;
    }
  }
  else if (0xfff < (int)unaff_A6[0xc]) {
    unaff_A6[0xc] = unaff_A6[0xc] + -0x1000;
  }
  unaff_A6[0xd] = unaff_A6[0xd] + -0x1000;
  candidate_143dc();
  candidate_427a();
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
    *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
    unaff_A6[0xd] = -unaff_A6[0xd];
    *(undefined2 *)((int)unaff_A6 + 0x26) = *(undefined2 *)((int)unaff_A6 + 0x92);
    candidate_411c();
    candidate_1acf2();
  }
  candidate_688d6();
  return;
}
