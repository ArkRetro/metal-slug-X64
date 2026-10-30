/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ae610.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined4 candidate_ae610(void)

{
  undefined2 extraout_D0u;
  undefined2 extraout_D0u_00;
  undefined2 uVar1;
  undefined2 extraout_D0u_01;
  undefined2 extraout_D0u_02;
  ushort in_D0w;
  undefined4 uVar2;
  char cVar3;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A6;
  bool bVar4;

  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_ae82e();
    uVar1 = extraout_D0u;
  }
  else {
    candidate_ae800();
    uVar1 = extraout_D0u_00;
  }
  uVar2 = CONCAT22(uVar1,in_D0w);
  if (*(char *)(unaff_A6 + 0x96) != '\0') {
    return uVar2;
  }
  if ((in_D0w & 0x40) == 0) {
    return uVar2;
  }
  if (*(char *)(unaff_A6 + 0x98) != '\0') {
    return uVar2;
  }
  if (*(char *)(unaff_A6 + 0x97) == '\0') {
    return uVar2;
  }
  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
    bVar4 = false;
    uVar2 = candidate_aa956();
    if (bVar4) {
      *(ushort *)(unaff_A6 + 0x76) = (ushort)uVar2 & 0xff;
      *(undefined1 *)(unaff_A6 + 0x98) = 0x20;
      candidate_40bc();
      candidate_411c();
      uVar2 = candidate_1acf2();
      *(undefined1 *)(extraout_A0 + 0x5e) = *(undefined1 *)(unaff_A6 + 0x5e);
    }
    uVar1 = (undefined2)((uint)uVar2 >> 0x10);
  }
  else {
    cVar3 = func_0x000c07e0();
    if (cVar3 != '\0') {
      bVar4 = false;
      candidate_c07f8();
      uVar1 = extraout_D0u_01;
      if (!bVar4) goto LAB_000ae700;
      candidate_c0808();
      candidate_ae800();
    }
    *(undefined1 *)(unaff_A6 + 0x98) = 0x20;
    candidate_40bc();
    *(undefined2 *)(extraout_A0_00 + 0x70) = *(undefined2 *)(unaff_A6 + 0x70);
    uVar1 = extraout_D0u_02;
  }
LAB_000ae700:
  return CONCAT22(uVar1,in_D0w);
}
