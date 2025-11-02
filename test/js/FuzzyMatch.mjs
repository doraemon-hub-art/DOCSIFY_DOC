import { fuzzy } from "fast-fuzzy";

const similarityScore = fuzzy("front yard", "Bacj Yard");

console.log(similarityScore);
