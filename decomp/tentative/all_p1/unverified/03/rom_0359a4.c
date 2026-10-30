/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0359a4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_359a4(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  char cVar4;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar5;

  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  candidate_4256();
  *unaff_A6 = 0x359be;
  candidate_35a28();
  uVar3 = *(ushort *)((int)unaff_A6 + 0x76);
  bVar5 = 0xfffe < uVar3;
  *(ushort *)((int)unaff_A6 + 0x76) = uVar3 + 1;
  candidate_427a();
  if (bVar5) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
    *unaff_A6 = candidate_356bc;
    return;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    candidate_1945c();
    if (bVar5) {
      candidate_4126();
      iVar2 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar2 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
    candidate_4c24();
    if (bVar5) {
      cVar4 = *(char *)((int)unaff_A6 + 0x5a);
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      if ((cVar4 == '\x01') || (cVar4 == '\t')) {
        unaff_A6[0xc] = 0x20000;
        unaff_A6[0xd] = 0x40000;
        *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
        if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
          unaff_A6[0xc] = -unaff_A6[0xc];
          *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
        }
        goto LAB_00035844;
      }
    }
    return;
  }
  candidate_1b1dc();
LAB_00035844:
  candidate_4256();
  *unaff_A6 = candidate_3585c;
  return;
}
