/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bc100.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_bc100(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  candidate_4256();
  unaff_A6[0xc] = 0xfffe4000;
  unaff_A6[0xd] = 0x70000;
  *(undefined1 *)(unaff_A6 + 0x1f) = 0;
  *unaff_A6 = 0xbc14a;
  *(undefined4 *)((int)unaff_A6 + 0x72) = unaff_A6[0xd];
  candidate_143dc();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *(char *)(unaff_A6 + 0x1f) = *(char *)(unaff_A6 + 0x1f) + '\x01';
    if (1 < *(byte *)(unaff_A6 + 0x1f)) {
      candidate_4126();
      iVar2 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar2 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
    iVar2 = *(int *)((int)unaff_A6 + 0x72);
    if (iVar2 < 0) {
      iVar2 = -iVar2;
    }
    unaff_A6[0xd] = iVar2 >> 1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
  }
  candidate_427a();
  return;
}
