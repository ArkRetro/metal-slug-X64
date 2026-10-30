/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x048a06.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_48a06(void)

{
  char *pcVar1;
  char cVar2;
  undefined1 uVar3;
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar4;

  uVar3 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x97) = uVar3;
  uVar3 = candidate_260c8();
  *(undefined1 *)(unaff_A6 + 0x26) = uVar3;
  *unaff_A6 = 0x48a2c;
  candidate_49efa();
  candidate_1ac38();
  candidate_1ad8c();
  bVar4 = false;
  candidate_49c52();
  if (bVar4) {
    if (*(char *)((int)unaff_A6 + 0x97) == '\0') {
      uVar3 = candidate_260c8();
      *(undefined1 *)((int)unaff_A6 + 0x97) = uVar3;
      *(ushort *)(unaff_A6 + 0x20) = (ushort)*(byte *)((int)unaff_A6 + 0x96) * 8 + 0xa0;
      candidate_40bc();
      *(undefined1 *)(extraout_A0 + 0x60) = 0;
      cVar2 = *(char *)(unaff_A6 + 0x26) + -1;
      *(char *)(unaff_A6 + 0x26) = cVar2;
      if (cVar2 == '\0') {
        *unaff_A6 = 0x489be;
      }
    }
    else {
      *(char *)((int)unaff_A6 + 0x97) = *(char *)((int)unaff_A6 + 0x97) + -1;
    }
  }
  candidate_427a();
  *(char *)((int)unaff_A6 + 0x96) = *(char *)((int)unaff_A6 + 0x96) + '\x03';
  pcVar1 = (char *)(*(int *)(unaff_A6[3] + 0xc) + 0x95);
  *pcVar1 = *pcVar1 + '\x01';
  candidate_49030();
  return;
}
