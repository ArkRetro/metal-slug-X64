/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0853e2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_853e2(void)

{
  byte bVar1;
  uint uVar2;
  char cVar3;
  uint uVar4;
  int unaff_A6;
  undefined8 uVar5;

  if (*(char *)(unaff_A6 + 0x8c) == '\0') {
    return;
  }
  cVar3 = *(char *)(unaff_A6 + 0x8c) << 1;
  do {
    cVar3 = cVar3 << 1;
  } while (cVar3 != '\0');
  uVar5 = candidate_29f2();
  uVar2 = (uint)((ulonglong)uVar5 >> 0x20);
  uVar4 = 7;
  while (((CONCAT31((int3)((ulonglong)uVar5 >> 8),*(undefined1 *)(unaff_A6 + 0x8c)) &
          1 << (uVar4 & 0x1f)) == 0 ||
         (bVar1 = (char)uVar2 - 1, uVar2 = (uint)bVar1, -1 < (char)bVar1))) {
    uVar4 = (uint)(byte)((char)uVar4 - 1);
  }
  *(byte *)(unaff_A6 + 0x8d) = *(byte *)(unaff_A6 + 0x8d) | '\x01' << (uVar4 & 7);
  return;
}
