/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a7ffe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a7ffe(void)

{
  short sVar1;
  byte bVar2;
  char cVar3;
  int extraout_A0;
  undefined4 *unaff_A6;

  candidate_2976();
  candidate_1aa28();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined1 *)(unaff_A6 + 0x1c) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0;
  *unaff_A6 = 0xa802a;
  sVar1 = *(short *)((int)unaff_A6 + 0x6e);
  *(short *)((int)unaff_A6 + 0x6e) = sVar1 + -1;
  if (sVar1 == 0) {
    candidate_40bc();
    *(undefined1 *)(extraout_A0 + 0x70) = *(undefined1 *)(unaff_A6 + 0x1c);
    bVar2 = *(byte *)(unaff_A6 + 0x1c);
    *(undefined2 *)(extraout_A0 + 0x72) = *(undefined2 *)((short)((ushort)bVar2 << 2) + 0xa7fd6);
    *(undefined2 *)((int)unaff_A6 + 0x6e) = *(undefined2 *)((short)((ushort)bVar2 << 2) + 0xa7fd8);
    cVar3 = *(char *)(unaff_A6 + 0x1c);
    *(byte *)(unaff_A6 + 0x1c) = cVar3 + 1U;
    if (9 < (byte)(cVar3 + 1U)) {
      *unaff_A6 = 0xa8076;
    }
  }
  if (*(char *)(unaff_A6 + 0x18) == '\0') {
    return;
  }
  candidate_9c0a();
  *unaff_A6 = candidate_a8096;
  return;
}
