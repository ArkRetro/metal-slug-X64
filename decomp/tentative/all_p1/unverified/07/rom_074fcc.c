/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x074fcc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_74fcc(void)

{
  int iVar1;
  undefined4 *unaff_A6;

  candidate_1ad6e();
  candidate_9ea8();
  do {
    *unaff_A6 = 0x74fee;
    if (*(char *)(unaff_A6[3] + 0x8a) == '\0') {
      return;
    }
    candidate_4256();
    *unaff_A6 = 0x75012;
    iVar1 = unaff_A6[3];
  } while (*(char *)(iVar1 + 0x8a) == '\0');
  *(undefined2 *)((int)unaff_A6 + 0x26) = *(undefined2 *)(iVar1 + 0x26);
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(iVar1 + 0x1e);
  candidate_1abc8();
  candidate_427a();
  return;
}
