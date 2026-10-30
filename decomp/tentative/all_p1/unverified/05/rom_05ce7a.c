/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05ce7a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5ce7a(void)

{
  short sVar1;
  byte bVar2;
  short sVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  if ((*(byte *)((int)unaff_A6 + 0x93) & 2) == 0) {
    candidate_4256();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      *(char *)((int)unaff_A6 + 0x8a) = '\b' - *(char *)((int)unaff_A6 + 0x8a);
    }
  }
  else {
    candidate_4256();
    *(char *)((int)unaff_A6 + 0x8a) = *(char *)((int)unaff_A6 + 0x8a) + -9;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
      *(char *)((int)unaff_A6 + 0x8a) = '\x06' - *(char *)((int)unaff_A6 + 0x8a);
    }
  }
  *(char *)((int)unaff_A6 + 0x8a) =
       *(char *)((int)unaff_A6 + 0x8a) + *(char *)((int)unaff_A6 + 0x8a);
  *(undefined1 *)(unaff_A6 + 0x22) = 0;
  *unaff_A6 = 0x5cef4;
  candidate_5e234();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x8a) & 0x1f) == *(byte *)(unaff_A6 + 0x22)) {
    *unaff_A6 = candidate_5cf2a;
  }
  bVar2 = *(byte *)(unaff_A6 + 0x22);
  bVar4 = 0xfe < bVar2;
  *(byte *)(unaff_A6 + 0x22) = bVar2 + 1;
  candidate_5e244();
  if (bVar4) {
    *unaff_A6 = candidate_5cd08;
  }
  candidate_5e6d2();
  sVar1 = *(short *)(unaff_A6 + 0x1f);
  sVar3 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar3;
  if (-1 >= sVar3) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  bVar4 = (*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0;
  if (!bVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0x1e;
  }
  bVar4 = bVar4 && (-1 < sVar3 && sVar1 == 0);
  candidate_5e7a8();
  if (bVar4) {
    *unaff_A6 = candidate_5d732;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
    *unaff_A6 = candidate_5db18;
  }
  bVar4 = false;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
