/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x089ed6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_89ed6(void)

{
  code *pcVar1;
  uint uVar2;
  int iVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_120f8();
  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8800;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8800;
                    /* WARNING: Switch is manually overridden */
  switch(*(short *)((int)unaff_A6 + 0x76) * 4) {
  case 0:
    candidate_4256();
    candidate_29c6();
    iVar3 = candidate_2e7c();
    unaff_A6[0xc] = iVar3 + unaff_A6[0xc];
    unaff_A6[0xd] = 0;
    *unaff_A6 = candidate_89f56;
    uVar2 = unaff_A6[0xc];
    uVar4 = uVar2 < (uint)((int)unaff_A6[0xc] >> 3);
    unaff_A6[0xc] = uVar2 - ((int)unaff_A6[0xc] >> 3);
    candidate_143dc();
    candidate_427a();
    if ((bool)uVar4) {
      candidate_4192();
      candidate_4126();
      iVar3 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar3 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
    return;
  case 4:
    candidate_4256();
    *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 0x18;
    unaff_A6[0xd] = 0;
    break;
  case 8:
    candidate_4256();
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfd;
    unaff_A6[0xd] = 0xfffc0000;
  }
  *unaff_A6 = candidate_89fbe;
  candidate_143dc();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_89fdc;
  }
  return;
}
