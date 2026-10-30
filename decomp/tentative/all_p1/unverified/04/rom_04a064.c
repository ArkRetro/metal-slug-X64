/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04a064.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4a064(void)

{
  int iVar1;
  char *pcVar2;
  ushort uVar3;
  int unaff_A6;
  bool bVar4;

  iVar1 = *(int *)(*(int *)(unaff_A6 + 0xc) + 0xc);
  if (*(char *)(iVar1 + 0x95) != '\0') {
    if (*(char *)(iVar1 + 0x99) <= *(char *)(iVar1 + 0x95)) {
      if ((short)((ushort)*(byte *)(iVar1 + 0x95) * *(short *)(iVar1 + 0x8c)) <=
          *(short *)(iVar1 + 0x8a)) {
        *(char *)(iVar1 + 0x99) = *(char *)(iVar1 + 0x99) + '\x01';
      }
      return;
    }
    return;
  }
  bVar4 = false;
  if (*(char *)(iVar1 + 0x99) != '\0') {
    return;
  }
  uVar3 = candidate_1ae8e();
  if (bVar4) {
    return;
  }
  bVar4 = uVar3 < 3;
  if (uVar3 != 3) {
    bVar4 = uVar3 == 0;
    if (uVar3 == 1) {
      candidate_4a15a();
      if (!bVar4) {
        return;
      }
    }
    else {
      candidate_4a15a();
      if (!bVar4) {
        return;
      }
    }
    iVar1 = *(int *)(*(int *)(unaff_A6 + 0xc) + 0xc);
    pcVar2 = (char *)(iVar1 + 0x99);
    *pcVar2 = *pcVar2 + '\x01';
    *(char *)(iVar1 + 0x98) = (char)*(undefined2 *)(unaff_A6 + 0x80);
    return;
  }
  candidate_4a15a();
  if ((!bVar4) && (candidate_4a15a(), !bVar4)) {
    return;
  }
  iVar1 = *(int *)(*(int *)(unaff_A6 + 0xc) + 0xc);
  pcVar2 = (char *)(iVar1 + 0x99);
  *pcVar2 = *pcVar2 + '\x01';
  *(char *)(iVar1 + 0x98) = (char)*(undefined2 *)(unaff_A6 + 0x80);
  return;
}
