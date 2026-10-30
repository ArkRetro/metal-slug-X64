/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a751c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a751c(void)

{
  short sVar2;
  uint uVar1;
  uint uVar3;
  undefined4 *unaff_A6;
  undefined8 uVar4;

  sVar2 = candidate_29c6();
  if (sVar2 < 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
  }
  if ((*(byte *)((int)unaff_A6 + 0x6e) & 0x40) == 0) {
    if (*(char *)((int)unaff_A6 + 0x7a) == '\0') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
  }
  else if (*(char *)((int)unaff_A6 + 0x7a) == '\0') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  if (*(char *)((int)unaff_A6 + 0x7a) == '\0') {
    candidate_9ea8();
  }
  else {
    candidate_9ea8();
  }
  if (*(char *)((int)unaff_A6 + 0x7b) == '\0') {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1110;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1110;
  }
  else {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x110f;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x110f;
  }
  uVar1 = (uint)*(ushort *)(unaff_A6 + 0xc);
  uVar3 = (uint)*(ushort *)(unaff_A6 + 0xd);
  if (*(ushort *)(unaff_A6 + 0xd) != 0 || *(ushort *)(unaff_A6 + 0xc) != 0) {
    candidate_2eb4();
    uVar4 = candidate_2e7c();
    uVar1 = -(int)((ulonglong)uVar4 >> 0x20);
    uVar3 = -(int)uVar4;
  }
  unaff_A6[0x1f] = uVar1;
  unaff_A6[0x20] = uVar3;
  unaff_A6[0xc] = (int)unaff_A6[0xc] >> 1;
  unaff_A6[0xd] = (int)unaff_A6[0xd] >> 1;
  sVar2 = candidate_29c6();
  unaff_A6[0xc] = sVar2 * 2 + unaff_A6[0xc];
  sVar2 = candidate_29c6();
  unaff_A6[0xd] = sVar2 * 2 + unaff_A6[0xd];
  candidate_1abc8();
  *unaff_A6 = candidate_a7654;
  return;
}
