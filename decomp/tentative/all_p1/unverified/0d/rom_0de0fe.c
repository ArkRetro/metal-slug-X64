/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0de0fe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_de0fe(void)

{
  short sVar1;
  char cVar2;
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_1abc8();
  sVar1 = candidate_49e8();
  if ((((sVar1 != 0) && (extraout_A0 != 0)) && (0x1f < *(int *)(extraout_A0 + 0x3c))) &&
     (*(int *)(extraout_A0 + 0x3c) < 0x101)) {
    *(undefined4 *)((int)unaff_A6 + 0x6e) = 0;
  }
  *(ushort *)(unaff_A6 + 0x21) = (*(byte *)((int)unaff_A6 + 0x86) & 0x1f) << 3;
  candidate_de262();
  *(undefined2 *)((int)unaff_A6 + 0x82) = 0xf;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0xde18e;
  bVar3 = false;
  if (*(int *)((int)unaff_A6 + 0x6e) == 0) {
    *unaff_A6 = 0xde210;
  }
  else {
    func_0x000040ac();
    if (bVar3) {
      cVar2 = candidate_2eb4();
      if ((char)*(undefined2 *)(unaff_A6 + 0x21) != cVar2) {
        if ((char)((char)*(undefined2 *)(unaff_A6 + 0x21) - cVar2) < '\0') {
          *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + 8;
        }
        else {
          *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + -8;
        }
        *(ushort *)(unaff_A6 + 0x21) = *(ushort *)(unaff_A6 + 0x21) & 0xff;
        candidate_de262();
        *(byte *)((int)unaff_A6 + 0x86) = (byte)*(undefined2 *)(unaff_A6 + 0x21) >> 3;
      }
    }
    else {
      *(undefined4 *)((int)unaff_A6 + 0x6e) = 0;
    }
  }
  sVar1 = *(short *)((int)unaff_A6 + 0x82) + -1;
  *(short *)((int)unaff_A6 + 0x82) = sVar1;
  if (sVar1 == 0) {
    *unaff_A6 = 0xde210;
  }
  candidate_1ac38();
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_de24a;
  }
  bVar3 = false;
  candidate_19504();
  if (bVar3) {
    *unaff_A6 = candidate_de06e;
  }
  return;
}
