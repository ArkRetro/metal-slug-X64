/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06778e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6778e(void)

{
  undefined2 uVar1;
  ushort uVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_1abc8();
  candidate_427a();
  uVar1 = *(undefined2 *)(unaff_A6 + 0xb);
  uVar2 = *(ushort *)(unaff_A6 + 0xb);
  bVar3 = CARRY2(*(ushort *)((int)unaff_A6 + 0x7a),uVar2);
  *(ushort *)(unaff_A6 + 0xb) = *(ushort *)((int)unaff_A6 + 0x7a) + uVar2;
  candidate_c899e();
  if (!bVar3) {
    candidate_c88b0();
    *unaff_A6 = candidate_677cc;
  }
  *(undefined2 *)(unaff_A6 + 0xb) = uVar1;
  bVar3 = false;
  if (*(ushort *)(unaff_A6 + 0x1c) == 0) {
    candidate_c8bec();
    if (bVar3) {
      *unaff_A6 = candidate_6780e;
    }
  }
  else if (*(ushort *)(unaff_A6 + 0x1c) <= *(ushort *)(unaff_A5 + 0x699e)) {
    *unaff_A6 = candidate_6780e;
  }
  return;
}
