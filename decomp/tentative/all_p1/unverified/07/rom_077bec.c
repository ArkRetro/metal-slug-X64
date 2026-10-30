/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x077bec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_77bec(void)

{
  byte bVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
  candidate_4256();
  unaff_A6[0x20] = 0;
  *unaff_A6 = 0x77c34;
  unaff_A6[10] = *(undefined4 *)(unaff_A6[3] + 0x28);
  candidate_1abc8();
  candidate_78d3a();
  *(undefined2 *)((int)unaff_A6 + 0x26) =
       *(undefined2 *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x9a) * 2) + 0x77c84);
  candidate_427a();
  bVar1 = *(byte *)(unaff_A6[3] + 0x9b);
  if (bVar1 == 2) {
    *unaff_A6 = candidate_77c8a;
  }
  bVar2 = bVar1 < 2;
  candidate_78d7a();
  if (bVar2) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
